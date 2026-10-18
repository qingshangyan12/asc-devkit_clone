/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License).
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/c_api/instr_impl/npu_arch_9201/cube_datamove_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_IMPL_H

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 9201

#include "c_api/utils/enum.h"
#include "impl/c_api/instr_impl/npu_arch_2201/utils_impl/utils_impl.h"

using asc_quant_mode = QuantMode_t;


constexpr uint8_t ASC_GM2L1_SID_DEFAULT = 0;

constexpr uint8_t ASC_GM2L1_DECOMP_MODE_DEFAULT = 0;


constexpr uint8_t RELU_POST_DEFAULT = 0;

constexpr bool CLIP_RELU_POST_DEFAULT = false;

constexpr uint8_t ELTWISE_OP_DEFAULT = 0;

constexpr uint64_t QUANT_POST_DEFAULT = 0;

// ==========asc_copy_l0c2l1==========
// half  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// int8_t  int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// uint8_t  int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, n_size, m_size, dst_stride, src_stride, false, static_cast<uint8_t>(enable_clip_relu_pre),
            static_cast<uint8_t>(unit_flag_mode), static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)),
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, false, false, false, false, false,
            false, false, false, enable_nz2dn);
    }
}

// int4b_t  float (s4)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, false,
            static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
            static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)), static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            false, false, false, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd,
            static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
            static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)), static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            false, false, false, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  int32_t (s4)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, false,
            static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
            static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)), static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            false, false, false, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, quant_pre_rnd,
            static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
            static_cast<QuantMode_t>(static_cast<uint64_t>(quant_pre_mode)), static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            false, false, false, false, false, false, false, false, enable_nz2dn);
    }
}

// ==========asc_prefetch_l2cache==========
// =======asc_prefetch_gm2l2cache=======
// int8_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ int8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ int8_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ int8_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// uint8_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ uint8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ uint8_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ uint8_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// hifloat8_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ hifloat8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ hifloat8_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ hifloat8_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// fp8_e5m2_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ fp8_e5m2_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ fp8_e5m2_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ fp8_e5m2_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// fp8_e4m3fn_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ fp8_e4m3fn_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ fp8_e4m3fn_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ fp8_e4m3fn_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// int16_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ int16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ int16_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ int16_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// uint16_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ uint16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ uint16_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ uint16_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// half
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ half* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ half*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ half*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// bfloat16_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ bfloat16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ bfloat16_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ bfloat16_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// int32_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ int32_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ int32_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ int32_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// uint32_t
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ uint32_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ uint32_t*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ uint32_t*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// float
__aicore__ inline void asc_prefetch_gm2l2cache(
    __gm__ float* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            (__cbuf__ float*)0, src, 0, burst_num, burst_len, 0, 0, true, false, 3, burst_src_stride, 0);
    }
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ float*)0, src, 0, burst_num, burst_len, 0, 0, false, false, 3, burst_src_stride, 0, false);
    }
}

// =======asc_prefetch_gm2l2cache_dn2nz=======
// int8_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ int8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ int8_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// uint8_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ uint8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ uint8_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// hifloat8_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ hifloat8_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// fp8_e5m2_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ fp8_e5m2_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// fp8_e4m3fn_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ fp8_e4m3fn_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// int16_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ int16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ int16_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// uint16_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ uint16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ uint16_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// half
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ half* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ half*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// bfloat16_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ bfloat16_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// int32_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ int32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ int32_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// uint32_t
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ uint32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ uint32_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// float
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(
    __gm__ float* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ float*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// =======asc_prefetch_gm2l2cache_nd2nz=======
// int8_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ int8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ int8_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// uint8_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ uint8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ uint8_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// hifloat8_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ hifloat8_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// fp8_e5m2_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ fp8_e5m2_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// fp8_e4m3fn_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ fp8_e4m3fn_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// int16_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ int16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ int16_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// uint16_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ uint16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ uint16_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// half
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ half* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ half*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// bfloat16_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ bfloat16_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// int32_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ int32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ int32_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// uint32_t
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ uint32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ uint32_t*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// float
__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz(
    __gm__ float* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            (__cbuf__ float*)0, src, 0, loop1_src_stride, 3, n_value, d_value, loop4_src_stride, false, false);
    }
}

// =======asc_prefetch_stop=======
__aicore__ inline void asc_prefetch_stop() { __prefetch_stop(); }

// ==========asc_copy_l12l0a==========
// hif4x2_t
__aicore__ inline void asc_copy_l12l0a(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(
            (__ca__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 0);
    }
}

// transpose
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(
            (__ca__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 1);
    }
}

// ==========asc_copy_l12l0b==========
// hif4x2_t
__aicore__ inline void asc_copy_l12l0b(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(
            (__cb__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 0);
    }
}

// transpose
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(
            (__cb__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 1);
    }
}

// ==========asc_copy_l0c2gm==========
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            false, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            quant_pre_rnd, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            false, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            quant_pre_rnd, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// ==========asc_copy_l0c2ub==========
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t burst_count, uint32_t burst_len, asc_channel_pad_mode pad_mode,
    bool enable_pre_load_ignore_sync, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_v2(
            dst, src, 0, burst_count, burst_len, static_cast<uint8_t>(pad_mode), 0, src_stride, dst_stride,
            enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0, bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0, bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

constexpr uint8_t ASC_L13D_RPT_REPEAT_TIMES_SHIFT = 16;

constexpr uint8_t ASC_L13D_RPT_DIRECTION_SHIFT = 24;

constexpr uint8_t ASC_L13D_RPT_DST_STRIDE_K_SHIFT = 32;

constexpr uint8_t ASC_L13D_RPT_DST_START_POS_M_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_H_SHIFT = 16;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_LEFT_SHIFT = 32;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_RIGHT_SHIFT = 40;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_TOP_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_BOTTOM_SHIFT = 56;

constexpr uint8_t ASC_L13D_FMATRIX_B_H_SHIFT = 16;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_LEFT_SHIFT = 32;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_RIGHT_SHIFT = 40;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_TOP_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_BOTTOM_SHIFT = 56;

constexpr uint8_t ASC_3D_PADDING_SHIFT_BIT = 8;

constexpr uint8_t ASC_L0C_COPY_CHANNEL_PARA_SHIFT = 48;

constexpr uint8_t ASC_L0C_COPY_NZ_PARA_SRC_SHIFT = 16;

constexpr uint8_t ASC_L0C_COPY_NZ_PARA_DST_SHIFT = 32;

constexpr uint8_t ASC_3D_PADDING_B_SHIFT_BIT = 8;

constexpr uint8_t ASC_L3D_RPT_B_REPEAT_TIMES_SHIFT = 16;

constexpr uint8_t ASC_L3D_RPT_B_DIRECTION_SHIFT = 24;

constexpr uint8_t ASC_L3D_RPT_B_DST_STRIDE_K_SHIFT = 32;

constexpr uint8_t ASC_L3D_RPT_B_DST_START_POS_M_SHIFT = 48;

__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_mx(dst, (__cbuf__ void*)src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_mx(dst, (__cbuf__ void*)src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(
            (__cb__ void*)dst, (__cbuf__ void*)src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap,
            src_frac_gap);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
        uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
        uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
        uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
        uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
        uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
        uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_align(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len,
        uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value,
        asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride,
        bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_dn2nz(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value,
        asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride,
        bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1_nd2nz(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
        uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0))
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, asc_channel_pad_mode pad_mode,
        uint64_t src_stride, uint32_t dst_stride))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_v2(dst, src, 0, n_burst, len_burst, pad_func_mode, 0, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, asc_channel_pad_mode pad_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_v2(dst, src, 0, n_burst, len_burst, static_cast<uint8_t>(pad_mode), 0, src_stride, dst_stride);
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
        uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
        uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

ASC_DEPRECATED(
    9.2.0, "2027/09/07",
    asc_copy_gm2l1(
        __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
        uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode))
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_l12fb(
    __fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size,
    uint16_t dst_gap_size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_fbuf(dst, src, n_burst, len_burst, src_gap_size, dst_gap_size);
    }
}

__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ bfloat16_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_l12bt(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ half* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    asc_copy_l12bt(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ float* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    asc_copy_l12bt(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ int32_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    asc_copy_l12bt(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

__aicore__ inline void asc_copy_l12ub(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, int8_t sub_blockid, uint16_t burst_count, uint16_t burst_len,
    uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_ubuf(dst_addr, src_addr, static_cast<bool>(sub_blockid), burst_count, burst_len, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_bf16(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_h(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_ui(dst, config.config, value);
    }
}

__aicore__ inline void asc_set_l12l0a_3d_padding(uint64_t config)
{
    if ASC_IS_AIC {
        set_padding(config);
    }
}

__aicore__ inline void asc_set_l12l0a_3d_padding(int8_t padding_value)
{
    uint64_t value = static_cast<uint64_t>(static_cast<uint8_t>(padding_value));
    asc_set_l12l0a_3d_padding((value << ASC_3D_PADDING_SHIFT_BIT) | value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(uint8_t padding_value)
{
    asc_set_l12l0a_3d_padding((static_cast<uint64_t>(padding_value) << ASC_3D_PADDING_SHIFT_BIT) | padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(int16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int16 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(uint16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint16 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(half padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_half = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(bfloat16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_bfloat16 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(int32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int32 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(uint32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint32 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(float padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_float = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(uint64_t config)
{
    if ASC_IS_AIC {
        set_padding_b(config);
    }
}

__aicore__ inline void asc_set_l12l0b_3d_padding(int8_t padding_value)
{
    uint64_t value = static_cast<uint64_t>(static_cast<uint8_t>(padding_value));
    asc_set_l12l0b_3d_padding((value << ASC_3D_PADDING_B_SHIFT_BIT) | value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(uint8_t padding_value)
{
    asc_set_l12l0b_3d_padding((static_cast<uint64_t>(padding_value) << ASC_3D_PADDING_B_SHIFT_BIT) | padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(int16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int16 = padding_value;
    asc_set_l12l0b_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(uint16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint16 = padding_value;
    asc_set_l12l0b_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(half padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_half = padding_value;
    asc_set_l12l0b_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(bfloat16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_bfloat16 = padding_value;
    asc_set_l12l0b_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(int32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int32 = padding_value;
    asc_set_l12l0b_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(uint32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint32 = padding_value;
    asc_set_l12l0b_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(float padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_float = padding_value;
    asc_set_l12l0b_3d_padding(bitcode.output);
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_config)
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag)
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = 0;
        config.relu_units = relu_pre;
        config.quant_units = quant_pre;
        config.unit_flag = enable_unit_flag;
        set_fpc(config.config);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_nz_para)
__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride)
{
    if ASC_IS_AIC {
        uint64_t config = (dst_nd_stride << 32) | (src_nd_stride << 16) | nd_num;
        set_loop3_para(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_config(uint64_t relu_pre_addr, uint64_t quant_pre_addr, bool is_clean_unit_flag)
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = 0;
        config.relu_units = relu_pre_addr;
        config.quant_units = quant_pre_addr;
        config.unit_flag = is_clean_unit_flag;
        set_fpc(config.config);
    }
}

__aicore__ inline void asc_set_l0c_copy_nz_para(
    uint16_t matrix_num, uint16_t src_nz_matrix_stride, uint32_t dst_matrix_stride)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_matrix_stride) << ASC_L0C_COPY_NZ_PARA_DST_SHIFT) |
                          (static_cast<uint64_t>(src_nz_matrix_stride) << ASC_L0C_COPY_NZ_PARA_SRC_SHIFT) | matrix_num;
        set_loop3_para(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config)
{
    if ASC_IS_AIC {
        set_quant_pre(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_prequant(float scale, uint16_t offset, bool is_signed)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(is_signed) << 46) | (static_cast<uint64_t>(offset & 0x1FF) << 37) |
                          static_cast<uint64_t>(*(reinterpret_cast<uint32_t*>(&scale)) & 0xFFFFE000u);
        set_quant_pre(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_channel_para(uint16_t src_nz_fractal_stride)
{
    if ASC_IS_AIC {
        uint64_t config = static_cast<uint64_t>(src_nz_fractal_stride) << ASC_L0C_COPY_CHANNEL_PARA_SHIFT;
        set_channel_para(config);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_channel_para)
__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config)
{
    if ASC_IS_AIC {
        set_channel_para(config);
    }
}

__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config)
{
    if ASC_IS_AIC {
        set_l3d_rpt_b(config);
    }
}

__aicore__ inline void asc_set_l13d_rpt_b(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_start_pos_m) << ASC_L3D_RPT_B_DST_START_POS_M_SHIFT) |
                          (static_cast<uint64_t>(dst_stride_k) << ASC_L3D_RPT_B_DST_STRIDE_K_SHIFT) |
                          (static_cast<uint64_t>(repeat_direction) << ASC_L3D_RPT_B_DIRECTION_SHIFT) |
                          (static_cast<uint64_t>(repeat_times) << ASC_L3D_RPT_B_REPEAT_TIMES_SHIFT) |
                          static_cast<uint64_t>(repeat_stride);
        set_l3d_rpt_b(config);
    }
}

__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config)
{
    if ASC_IS_AIC {
        set_mte2_nz_para(config);
    }
}

__aicore__ inline void asc_set_gm2l1_nz_para(
    uint16_t matrix_num, uint16_t dst_nz_n_stride, uint16_t dst_nz_c0_stride, uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_nz_matrix_stride) << 48) |
                          (static_cast<uint64_t>(dst_nz_c0_stride) << 32) |
                          (static_cast<uint64_t>(dst_nz_n_stride) << 16) | static_cast<uint64_t>(matrix_num);
        set_mte2_nz_para(config);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
__aicore__ inline void asc_set_l13d_padding(uint64_t config)
{
    if ASC_IS_AIC {
        uint64_t paddingValue = 0;
        uint64_t padValueShiftBit = 8;
        paddingValue = (config << padValueShiftBit) | (config & 0xFF);
        set_padding(paddingValue);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
__aicore__ inline void asc_set_l13d_padding(half config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_half = config;
        set_padding(bitcode.output);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
__aicore__ inline void asc_set_l13d_padding(int16_t config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_int16_t = config;
        set_padding(bitcode.output);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0a_3d_padding)
__aicore__ inline void asc_set_l13d_padding(uint16_t config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_uint16_t = config;
        set_padding(bitcode.output);
    }
}

__aicore__ inline void asc_set_gm2l1_padding(uint64_t config)
{
    if ASC_IS_AIC {
        set_padding(config);
    }
}

__aicore__ inline void asc_set_gm2l1_padding(int8_t padding_value)
{
    uint64_t value = static_cast<uint64_t>(static_cast<uint8_t>(padding_value));
    asc_set_l12l0a_3d_padding((value << ASC_3D_PADDING_SHIFT_BIT) | value);
}

__aicore__ inline void asc_set_gm2l1_padding(uint8_t padding_value)
{
    asc_set_l12l0a_3d_padding((static_cast<uint64_t>(padding_value) << ASC_3D_PADDING_SHIFT_BIT) | padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding(int16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int16 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_gm2l1_padding(uint16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint16 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_gm2l1_padding(half padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_half = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_gm2l1_padding(bfloat16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_bfloat16 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_gm2l1_padding(int32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int32 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_gm2l1_padding(uint32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint32 = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

__aicore__ inline void asc_set_gm2l1_padding(float padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_float = padding_value;
    asc_set_l12l0a_3d_padding(bitcode.output);
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l12l0b_3d_padding)
__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config)
{
    if ASC_IS_AIC {
        set_padding_b(config);
    }
}

[[deprecated("Deprecated since 9.2.0, Will be removed after 2027/09/07")]]
__aicore__ inline void asc_set_l0c2gm_quant_post(uint64_t config)
{
    if ASC_IS_AIC {
        set_quant_post(config);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_relu_alpha)
__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config)
{
    if ASC_IS_AIC {
        set_relu_alpha(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_relu_alpha(float scalar_relu_pre_alpha)
{
    if ASC_IS_AIC {
        set_relu_alpha(static_cast<uint64_t>(*(reinterpret_cast<uint32_t*>(&scalar_relu_pre_alpha)) & 0xFFFFE000u));
    }
}

__aicore__ inline void asc_set_gm2l1_loop_size(uint64_t loop1_size, uint64_t loop2_size)
{
    if ASC_IS_AIC {
        asc_gm2l1_loop_size_config config;
        config.loop1_size = loop1_size;
        config.loop2_size = loop2_size;
        set_loop_size_outtol1(config.config);
    }
}

__aicore__ inline void asc_set_gm2l1_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    if ASC_IS_AIC {
        asc_gm2l1_loop_stride_config config;
        config.src_stride = loop1_src_stride;
        config.dst_stride = loop1_dst_stride;
        set_loop1_stride_outtol1(config.config);
    }
}

__aicore__ inline void asc_set_gm2l1_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    if ASC_IS_AIC {
        asc_gm2l1_loop_stride_config config;
        config.src_stride = loop2_src_stride;
        config.dst_stride = loop2_dst_stride;
        set_loop2_stride_outtol1(config.config);
    }
}

__aicore__ inline void asc_set_gm2l1_pad(uint32_t pad_val)
{
    if ASC_IS_AIC {
        set_pad_val_outtol1(pad_val);
    }
}

__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config)
{
    if ASC_IS_AIC {
        set_l3d_rpt(config.config);
    }
}

__aicore__ inline void asc_set_l13d_rpt(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_start_pos_m) << ASC_L13D_RPT_DST_START_POS_M_SHIFT) |
                          (static_cast<uint64_t>(dst_stride_k) << ASC_L13D_RPT_DST_STRIDE_K_SHIFT) |
                          (static_cast<uint64_t>(repeat_direction) << ASC_L13D_RPT_DIRECTION_SHIFT) |
                          (static_cast<uint64_t>(repeat_times) << ASC_L13D_RPT_REPEAT_TIMES_SHIFT) |
                          static_cast<uint64_t>(repeat_stride);
        set_l3d_rpt(config);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix(asc_l13d_fmatrix_config& config)
{
    if ASC_IS_AIC {
        set_fmatrix(config.config);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom)
{
    if ASC_IS_AIC {
        uint64_t config = static_cast<uint64_t>(fmatrix_w) |
                          (static_cast<uint64_t>(fmatrix_h) << ASC_L13D_FMATRIX_H_SHIFT) |
                          (static_cast<uint64_t>(pad_left) << ASC_L13D_FMATRIX_PAD_LEFT_SHIFT) |
                          (static_cast<uint64_t>(pad_right) << ASC_L13D_FMATRIX_PAD_RIGHT_SHIFT) |
                          (static_cast<uint64_t>(pad_top) << ASC_L13D_FMATRIX_PAD_TOP_SHIFT) |
                          (static_cast<uint64_t>(pad_bottom) << ASC_L13D_FMATRIX_PAD_BOTTOM_SHIFT);
        set_fmatrix(config);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix_b(asc_l13d_fmatrix_config& config)
{
    if ASC_IS_AIC {
        set_fmatrix_b(config.config);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix_b(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom)
{
    if ASC_IS_AIC {
        uint64_t config = static_cast<uint64_t>(fmatrix_w) |
                          (static_cast<uint64_t>(fmatrix_h) << ASC_L13D_FMATRIX_B_H_SHIFT) |
                          (static_cast<uint64_t>(pad_left) << ASC_L13D_FMATRIX_B_PAD_LEFT_SHIFT) |
                          (static_cast<uint64_t>(pad_right) << ASC_L13D_FMATRIX_B_PAD_RIGHT_SHIFT) |
                          (static_cast<uint64_t>(pad_top) << ASC_L13D_FMATRIX_B_PAD_TOP_SHIFT) |
                          (static_cast<uint64_t>(pad_bottom) << ASC_L13D_FMATRIX_B_PAD_BOTTOM_SHIFT);
        set_fmatrix_b(config);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_lrelu_alpha)
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(config);
    }
}

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_set_l0c_copy_lrelu_alpha)
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_lrelu_alpha(float scalar_relu_pre_alpha)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(scalar_relu_pre_alpha);
    }
}


// ========== uint8_t l2 cache mode compatible overloads (A6/9201) ==========
// Mirrors the A5(3510) uint8_t l2_cache_ctl overload group so that callers passing a plain
// uint8_t cache mode (e.g. ops-tensor tile paths) compile unchanged on A6(9201).
// Each overload forwards to the asc_load_l2_cache_mode version above.

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
        static_cast<asc_load_l2_cache_mode>(l2_cache_mode), src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, uint8_t l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz(
        dst, src, src_d_value, static_cast<asc_load_l2_cache_mode>(l2_cache_mode), n_value, d_value,
        src_nd_matrix_stride, enable_small_c0);
}

#endif // defined(__NPU_ARCH__) && __NPU_ARCH__ == 9201

#endif // IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_IMPL_H

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
