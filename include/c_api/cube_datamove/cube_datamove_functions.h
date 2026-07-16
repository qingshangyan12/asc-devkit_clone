/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/**
 * @file cube_datamove_functions.h
 * @brief Soft-emulation / usability wrappers for cube data-move APIs.
 * IMPORTANT:
 * - Interfaces in this header are NOT native cube INTRI instructions.
 * - Provided for software emulation (soft-sim) ease of use, primarily:
 *   - synchronized variants (*_sync)
 *   - compact size-based overloads (..., uint32_t size)
 * - For performance-critical device code, use the corresponding asynchronous
 *   native APIs in cube_datamove_functions.h.
 * @see cube_datamove_functions.h
 * @see docs/zh/api/SIMD-API/C-API/cube_datamove/
 */
#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "cube_datamove_functions.h cannot be used with compile flag --enable-simt enabled."
#endif

#ifndef INCLUDE_C_API_CUBE_DATAMOVE_CUBE_DATAMOVE_FUNCTIONS_H
#define INCLUDE_C_API_CUBE_DATAMOVE_CUBE_DATAMOVE_FUNCTIONS_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl.h"

#endif

/**
 * @ingroup CubeDataMove
 * @brief Copy matrix data from L1 Buffer to L0A Buffer (soft-emulation wrapper).
 * Supports 2D fractal transfers and 3D Load3D-style loads used by cube MMA.
 * Typed overloads share the same data path; choose the signature that matches
 * the required layout (2D vs 3D).
 * @param[out] dst                 Destination address in L0A (__ca__).
 * @param[in]  src                 Source address in L1 (__cbuf__).
 * @param[in]  start_index         (2D) Starting fractal index in src; unit 512B.
 * @param[in]  repeat              (2D) Iteration count; each iteration moves 512B.
 * @param[in]  src_stride          (2D) Gap between consecutive source fractals; unit 512B.
 * @param[in]  dst_gap             (2D) Gap between consecutive destination blocks; unit 512B.
 * @param[in]  k_extension         (3D) Extent along K for the Load3D window.
 * @param[in]  m_extension         (3D) Extent along M for the Load3D window.
 * @param[in]  k_start_pt          (3D) Start position along K.
 * @param[in]  m_start_pt          (3D) Start position along M.
 * @param[in]  stride_w/h          (3D) Convolution / sampling stride in W/H.
 * @param[in]  filter_w/h          (3D) Filter kernel size in W/H.
 * @param[in]  dilation_filter_w/h (3D) Filter dilation in W/H.
 * @param[in]  enable_filter_*     (3D) Enable extended filter-size behavior.
 * @param[in]  enable_transpose    (3D) Enable transpose while loading.
 * @param[in]  enable_f_matrix_ctrl (3D) Use configured F-matrix sideband.
 * @param[in]  channel_size        (3D) Channel size for the Load3D transfer.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 * @note Configure Load3D sideband via asc_set_l13d_* before 3D overloads when required.
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup CubeDataMove
 * @brief Copy matrix data from L1 Buffer to L0B Buffer (soft-emulation wrapper).
 * Supports 2D fractal transfers and 3D Load3D-style weight/feature loads for cube MMA.
 * @param[out] dst                 Destination address in L0B (__cb__).
 * @param[in]  src                 Source address in L1 (__cbuf__).
 * @param[in]  start_index         (2D) Starting fractal index in src; unit 512B.
 * @param[in]  repeat              (2D) Iteration count; each iteration moves 512B.
 * @param[in]  src_stride          (2D) Gap between consecutive source fractals; unit 512B.
 * @param[in]  dst_gap             (2D) Gap between consecutive destination blocks; unit 512B.
 * @param[in]  k_extension         (3D) Extent along K for the Load3D window.
 * @param[in]  m_extension         (3D) Extent along M for the Load3D window.
 * @param[in]  k_start_pt/m_start_pt (3D) Start positions along K/M.
 * @param[in]  stride_w/h, filter_w/h, dilation_filter_w/h
 *                                 (3D) Geometry controls for the load window.
 * @param[in]  enable_transpose    (3D) Enable transpose while loading.
 * @param[in]  enable_f_matrix_ctrl (3D) Use configured F-matrix sideband.
 * @param[in]  channel_size        (3D) Channel size for the Load3D transfer.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup CubeDataMove
 * @brief Initialize L0A Buffer with a scalar fill value (soft-emulation wrapper).
 * @param[out] dst     Destination address in L0A (__ca__).
 * @param[in]  value   Scalar fill pattern (typed value or uint32_t bit pattern).
 * @param[in]  config  Fill extent / control (asc_fill_value_config).
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

/**
 * @ingroup CubeDataMove
 * @brief Initialize L1 Buffer with a scalar fill value (soft-emulation wrapper).
 * @param[out] dst     Destination address in L1 (__cbuf__).
 * @param[in]  value   Scalar fill pattern (typed value or uint32_t bit pattern).
 * @param[in]  config  Fill extent / control (asc_fill_value_config).
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(
    __cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 Buffer to Fixpipe Buffer (soft-emulation wrapper).
 * @param[out] dst            Destination address in FB (__fbuf__).
 * @param[in]  src            Source address in L1 (__cbuf__).
 * @param[in]  burst_count    Number of bursts.
 * @param[in]  burst_len      Length of each burst.
 * @param[in]  src_gap_size   Gap between consecutive source bursts.
 * @param[in]  dst_gap_size   Gap between consecutive destination bursts.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 * @param[in] size  Byte / element count for the compact size overload.
 */
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size);

__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size);

/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 Buffer to Bias Table (soft-emulation wrapper).
 * @param[out] dst            Destination Bias Table address (encoded uint64_t).
 * @param[in]  src            Source address in L1 (__cbuf__).
 * @param[in]  conv_control   Conversion / format control for the transfer.
 * @param[in]  burst_count    Number of bursts.
 * @param[in]  burst_len      Length of each burst.
 * @param[in]  source_gap     Gap between consecutive source bursts.
 * @param[in]  dst_gap        Gap between consecutive destination bursts.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 * @param[in] size  Byte / element count for the compact size overload.
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

/**
 * @ingroup CubeDataMove
 * @brief Initialize L0B Buffer with a scalar fill value (soft-emulation wrapper).
 * @param[out] dst     Destination address in L0B (__cb__).
 * @param[in]  value   Scalar fill pattern (typed value or uint32_t bit pattern).
 * @param[in]  config  Fill extent / control (asc_fill_value_config).
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup CubeDataMove
 * @brief GM->L1 copy with pad / pack modes pad1..pad8 (soft-emulation wrapper).
 * Each pad mode inserts or compacts data within 32B units while copying GM->L1.
 * @param[out] dst          Destination address in L1 (__cbuf__).
 * @param[in]  src          Source address in GM (__gm__).
 * @param[in]  burst_count  Number of bursts (burst overload).
 * @param[in]  burst_len    Length of each burst (burst overload).
 * @param[in]  src_gap      Gap between consecutive source bursts.
 * @param[in]  dst_gap      Gap between consecutive destination bursts.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 * @note Size-based overloads for these modes live in cube_datamove_functions.h.
 * @param[in] size  Byte / element count for the compact size overload.
 */
__aicore__ inline void asc_copy_gm2l1_pad1(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad2(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad2_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad3(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad3_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad4(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad4_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad5(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad5_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad6(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad6_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad7(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad7_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad8(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad8_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup CubeDataMove
 * @brief Convert ND-layout GM data to NZ layout in L1 (soft-emulation wrapper).
 * @param[out] dst                   Destination NZ address in L1 (__cbuf__).
 * @param[in]  src                   Source ND address in GM (__gm__).
 * @param[in]  nd_num                Number of ND matrices / tiles.
 * @param[in]  n_value               N dimension of each ND matrix.
 * @param[in]  d_value               D dimension of each ND matrix.
 * @param[in]  src_nd_matrix_stride  Stride between consecutive ND matrices in src.
 * @param[in]  src_d_value           Source stride along D (elements).
 * @param[in]  dst_nz_c0_stride      Destination NZ C0 stride.
 * @param[in]  dst_nz_n_stride       Destination NZ N stride.
 * @param[in]  dst_nz_matrix_stride  Destination stride between NZ matrices.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup CubeDataMove
 * @brief Copy matrix data from Global Memory to L1 Buffer (soft-emulation wrapper).
 * Burst / typed 2D overloads without specialized pad or ND/DN conversion.
 * @param[out] dst          Destination address in L1 (__cbuf__).
 * @param[in]  src          Source address in GM (__gm__).
 * @param[in]  burst_count  Number of bursts (burst overload).
 * @param[in]  burst_len    Length of each burst (burst overload).
 * @param[in]  src_gap      Gap between consecutive source bursts.
 * @param[in]  dst_gap      Gap between consecutive destination bursts.
 * @param[in]  base_idx     (typed 2D) Starting index / base fractal.
 * @param[in]  repeat       (typed 2D) Iteration count.
 * @param[in]  src_stride   (typed 2D) Source stride between iterations.
 * @param[in]  dst_gap      (typed 2D) Destination gap between iterations.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 * @note Compact (..., uint32_t size) overloads are in cube_datamove_functions.h.
 * @param[in] size  Byte / element count for the compact size overload.
 */
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup CubeDataMove
 * @brief Copy matrix data from Global Memory directly to L0A (soft-emulation wrapper).
 * @param[out] dst         Destination address in L0A (__ca__).
 * @param[in]  src         Source address in GM (__gm__).
 * @param[in]  base_idx    Starting index / base fractal.
 * @param[in]  repeat      Iteration count.
 * @param[in]  src_stride  Source stride between iterations.
 * @param[in]  dst_gap     Destination gap between iterations.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup CubeDataMove
 * @brief Copy matrix data from Global Memory directly to L0B (soft-emulation wrapper).
 * @param[out] dst         Destination address in L0B (__cb__).
 * @param[in]  src         Source address in GM (__gm__).
 * @param[in]  base_idx    Starting index / base fractal.
 * @param[in]  repeat      Iteration count.
 * @param[in]  src_stride  Source stride between iterations.
 * @param[in]  dst_gap     Destination gap between iterations.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup CubeDataMove
 * @brief Transpose / transformed copy from L1 to L0A (soft-emulation wrapper).
 * @param[out] dst                    Destination address in L0A (__ca__).
 * @param[in]  src                    Source address in L1 (__cbuf__).
 * @param[in]  index_id               Source fractal / index selector.
 * @param[in]  repeat                 Iteration count.
 * @param[in]  src_stride             Source stride between iterations.
 * @param[in]  dst_gap                Destination gap between iterations.
 * @param[in]  enable_addr_decrement  Enable address decrement mode when present.
 * @param[in]  dst_frac_gap           Destination fractal gap when present.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup CubeDataMove
 * @brief Sparse-indexed copy from L1 to L0B (soft-emulation wrapper).
 * @param[out] dst          Destination address in L0B (__cb__).
 * @param[in]  src          Source data address in L1 (__cbuf__).
 * @param[in]  index        Sparse index table address in L1 (__cbuf__).
 * @param[in]  start_index  Starting index into the sparse table.
 * @param[in]  repeat       Iteration count.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12l0b_sparse_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat);

/**
 * @ingroup CubeDataMove
 * @brief Transpose / transformed copy from L1 to L0B (soft-emulation wrapper).
 * @param[out] dst                    Destination address in L0B (__cb__).
 * @param[in]  src                    Source address in L1 (__cbuf__).
 * @param[in]  index_id               Source fractal / index selector.
 * @param[in]  repeat                 Iteration count.
 * @param[in]  src_stride             Source stride between iterations.
 * @param[in]  dst_gap                Destination gap between iterations.
 * @param[in]  enable_addr_decrement  Enable address decrement mode when present.
 * @param[in]  dst_frac_gap           Destination fractal gap when present.
 * @param[in]  src_frac_gap           Source fractal gap when present (arch-specific).
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup CubeDataMove
 * @brief Store cube results from L0C to Global Memory via fixpipe (soft-emulation wrapper).
 * Optionally applies quantization / ReLU / channel-split / NZ2ND post-processing.
 * @param[out] dst               Destination address in GM (__gm__).
 * @param[in]  src               Source address in L0C (__cc__).
 * @param[in]  n_size            Source NZ size along N (constraints depend on
 *                               channel_split / nz2nd_en).
 * @param[in]  m_size            Source NZ size along M.
 * @param[in]  dst_stride_dst_d  Destination stride (NZ Z-gap or ND row length).
 * @param[in]  src_stride        Source stride in L0C.
 * @param[in]  unit_flag_mode    Unit-flag / dual-issue control.
 * @param[in]  quant_pre         Pre-quantization mode / parameter.
 * @param[in]  relu_pre          Pre-ReLU mode.
 * @param[in]  channel_split     Enable channel-split layout.
 * @param[in]  nz2nd_en          Enable NZ->ND conversion on store.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 * @note Pre-configure related fixpipe state via asc_set_l0c_* / lrelu alpha when needed.
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup CubeDataMove
 * @brief Store cube results from L0C to L1 via fixpipe (soft-emulation wrapper).
 * @param[out] dst        Destination address in L1 (__cbuf__).
 * @param[in]  src        Source address in L0C (__cc__).
 * @param[in]  n_size     Source size along N.
 * @param[in]  m_size     Source size along M.
 * @param[in]  ...        Destination / source strides and fixpipe controls as per overload.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
    bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup CubeDataMove
 * @brief Load image data into CBUF / L1 for cube consumption (soft-emulation wrapper).
 * @param[out] dst  Destination address in CBUF/L1.
 * @param[in]  src  Source image address in GM / reserved encoding as per overload.
 * @param[in]  ...  Image geometry / stride controls as per overload.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_load_image_to_cbuf_sync(
    __cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

__aicore__ inline void asc_load_image_to_cbuf_sync(
    __cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

/**
 * @ingroup CubeDataMove
 * @brief Aligned / multi-loop GM->L1 matrix copy (soft-emulation wrapper).
 * @param[out] dst  Destination address in L1 (__cbuf__).
 * @param[in]  src  Source address in GM (__gm__).
 * @param[in]  ...  Align / loop / stride / pad parameters as per overload.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 * @note Related loop/pad sideband is configured by asc_set_gm2l1_*.
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl,
    uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl,
    uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

/**
 * @ingroup CubeDataMove
 * @brief Convert DN-layout GM data to NZ layout in L1 (soft-emulation wrapper).
 * @param[out] dst  Destination NZ address in L1 (__cbuf__).
 * @param[in]  src  Source DN address in GM (__gm__).
 * @param[in]  ...  DN shape / stride / NZ destination stride parameters as per overload.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t burst_count, uint32_t burst_len, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup CubeDataMove
 * @brief MX-format related copy from L1 to L0A (soft-emulation wrapper).
 * @param[out] dst         Destination L0A / MX destination encoding.
 * @param[in]  src         Source address in L1 (__cbuf__), typically MX scale data.
 * @param[in]  x_start_pos Starting X position.
 * @param[in]  y_start_pos Starting Y position.
 * @param[in]  x_step      Step along X.
 * @param[in]  y_step      Step along Y.
 * @param[in]  src_stride  Source stride.
 * @param[in]  dst_stride  Destination stride.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12l0a_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup CubeDataMove
 * @brief MX-format related copy from L1 to L0B (soft-emulation wrapper).
 * @param[out] dst         Destination L0B / MX destination encoding.
 * @param[in]  src         Source address in L1 (__cbuf__), typically MX scale data.
 * @param[in]  x_start_pos Starting X position.
 * @param[in]  y_start_pos Starting Y position.
 * @param[in]  x_step      Step along X.
 * @param[in]  y_step      Step along Y.
 * @param[in]  src_stride  Source stride.
 * @param[in]  dst_stride  Destination stride.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12l0b_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend,
    bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float* src, uint32_t size);

/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 Buffer to Unified Buffer (soft-emulation wrapper).
 * @param[out] dst  Destination address in UB (__ubuf__).
 * @param[in]  src  Source address in L1 (__cbuf__).
 * @param[in]  ...  Burst / stride controls as per overload.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12ub_sync(
    __ubuf__ void* dst, __cbuf__ void* src, bool sub_blockid, uint16_t burst_count, uint16_t burst_len,
    uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 Buffer to L0C Buffer (soft-emulation wrapper).
 * @param[out] dst  Destination address in L0C (__cc__).
 * @param[in]  src  Source address in L1 (__cbuf__).
 * @param[in]  ...  Shape / stride controls as per overload.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ half* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ float* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ float* dst, __cbuf__ float* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 Buffer to Global Memory (soft-emulation wrapper).
 * @param[out] dst  Destination address in GM (__gm__).
 * @param[in]  src  Source address in L1 (__cbuf__).
 * @param[in]  ...  Burst / stride controls as per overload.
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l12gm_sync(
    __gm__ void* dst, __cbuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e4m3fn_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e5m2_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e5m2_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ hifloat8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_ctrl, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

/**
 * @ingroup CubeDataMove
 * @brief Quantize and store cube results from L0C to Unified Buffer (soft-emulation wrapper).
 * Applies pre/post quantization and related fixpipe controls, then moves to UB.
 * @param[out] dst                 Destination address in UB (__ubuf__).
 * @param[in]  src                 Source address in L0C (__cc__).
 * @param[in]  n_size / m_size     Source tile sizes along N/M.
 * @param[in]  dst_stride          Destination stride in UB.
 * @param[in]  src_stride          Source stride in L0C.
 * @param[in]  dual_dst_ctrl       Dual-destination control.
 * @param[in]  sub_blockid         Sub-block targeting control.
 * @param[in]  clip_relu_pre       Pre clip-ReLU mode.
 * @param[in]  unit_flag           Unit-flag control.
 * @param[in]  quant_pre_mode      Pre-quantization mode.
 * @param[in]  relu_pre_mode       Pre-ReLU mode.
 * @param[in]  split_en            Enable split / channel-split behavior.
 * @param[in]  enable_nz2dn        Enable NZ->DN conversion when present.
 * @param[in]  quant_post_mode     Post-quantization mode.
 * @param[in]  relu_post_mode      Post-ReLU mode.
 * @param[in]  enable_clip_relu_post_mode
 *                                 Enable post clip-ReLU.
 * @param[in]  eltwise_op          Elementwise post-op selector.
 * @param[in]  enable_antiquant    Enable anti-quantization path.
 * @param[in]  enable_c0_pad       Enable C0 padding.
 * @param[in]  enable_broadcast    Enable broadcast.
 * @param[in]  enable_nz2dn        Enable NZ2DN (alias on some overloads).
 * @note Soft-emulation / usability wrapper — NOT a native cube INTRI instruction.
 *       Prefer the matching async API in cube_datamove_functions.h for performance-critical paths.
 * @return None.
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e5m2_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e5m2_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ hifloat8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
    bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t clip_relu_pre, uint8_t unit_flag, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool split_en, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode,
    bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad,
    bool enable_broadcast, bool enable_nz2dn);

#endif

#endif
