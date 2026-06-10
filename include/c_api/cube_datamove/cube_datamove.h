/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif     

#ifndef INCLUDE_C_API_CUBE_DATAMOVE_CUBE_DATAMOVE_H
#define INCLUDE_C_API_CUBE_DATAMOVE_CUBE_DATAMOVE_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "instr_impl/npu_arch_2201/cube_datamove_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "instr_impl/npu_arch_3510/cube_datamove_impl.h"

#endif

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to Feature buffer
 * @param [out] dst Destination buffer in Feature Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] source_gap Gap between source bursts
 * @param [in] dst_gap Gap between destination bursts
 * @details Performs data copy from Cube Buffer (L1) to Feature Buffer.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint16_t burst_count, uint16_t burst_len,
                                      uint16_t source_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to BT buffer
 * @param [out] dst Destination address in BT buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] conv_control Conversion control parameter
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] source_gap Gap between source data blocks
 * @param [in] dst_gap Gap between destination data blocks
 * @details Transfers data from Cube Buffer (L1) to BT (Bias/Threshold) Buffer.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t source_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Fill L1 buffer with specified value
 * @brief Supports data types: half, float, int16_t, int32_t, uint16_t, uint32_t, bfloat16_t
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] value Value to fill the buffer with
 * @param [in] config Fill value configuration structure
 * @details Fills Cube Buffer (L1) with a constant value for initialization.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

/*
 * @ingroup CubeDataMove
 * @brief Fill L0A buffer with specified value
 * @brief Supports data types: half, float, int16_t, int32_t, uint16_t, uint32_t, bfloat16_t
 * @param [out] dst Destination buffer in Cube L0A Buffer
 * @param [in] value Value to fill the buffer with
 * @param [in] config Fill value configuration structure
 * @details Fills Cube L0A Buffer with a constant value for initialization.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_fill_l0a(__ca__ half* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ float* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ int16_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ int32_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);
__aicore__ inline void asc_fill_l0a(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);


/*
 * @ingroup CubeDataMove
 * @brief Fill L0B buffer with specified value
 * @brief Supports data types: half, float, int16_t, int32_t, uint16_t, uint32_t, bfloat16_t
 * @param [out] dst Destination buffer in Cube L0B Buffer
 * @param [in] value Value to fill the buffer with
 * @param [in] config Fill value configuration structure
 * @details Fills Cube L0B Buffer with a constant value for initialization.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_fill_l0b(__cb__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

/*
 * @ingroup CubeDataMove
 * @brief Set 3d copy repeat configuration
 * @param [in] config Load3D configuration structure reference
 * @details Configures repeat parameters for 3d copy operations.
 *          Used for image data loading in convolution operations.
 */
__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config);

/*
 * @ingroup CubeDataMove
 * @brief Set 3d copy feature matrix configuration
 * @param [in] config Feature matrix configuration structure reference
 * @details Configures feature matrix parameters for 3d copy operations.
 *          Used for feature extraction in convolution operations.
 */
__aicore__ inline void asc_set_l13d_fmatrix(asc_l13d_fmatrix_config& config);

/*
 * @ingroup CubeDataMove
 * @brief Set Load3D feature matrix configuration for B matrix
 * @param [in] config Feature matrix configuration structure reference
 * @details Configures feature matrix parameters for Load3D B matrix operations.
 */
__aicore__ inline void asc_set_l13d_fmatrix_b(asc_l13d_fmatrix_config& config);

/*
 * @ingroup CubeDataMove
 * @brief Set padding attribute description for 3D format data copy interfaces
 * @brief Supports data types: uint64_t, half, int16_t, uint16_t
 * @param [in] config Padding value for asc_copy_l12l0a interface when configuring fill values
 * @details Sets the padding attribute description used when calling asc_copy_l12l0a interface to configure fill values.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_set_l13d_padding(uint64_t config);
__aicore__ inline void asc_set_l13d_padding(half config);
__aicore__ inline void asc_set_l13d_padding(int16_t config);
__aicore__ inline void asc_set_l13d_padding(uint16_t config);

/*
 * @ingroup CubeDataMove
 * @brief Set Leaky ReLU alpha parameter for L0C to GM copy
 * @brief Supports data types: half, float
 * @param [in] config Leaky ReLU alpha value
 * @details Sets the alpha parameter for Leaky ReLU activation during L0C to GM copy operations.
 */
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config);
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer (2201 architecture)
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Global Memory
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] src_gap Gap between source bursts
 * @param [in] dst_gap Gap between destination bursts
 * @details Copies data from Global Memory (__gm__) to Cube Buffer (__cbuf__) without padding.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE1 (2201 architecture)
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Global Memory
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] src_gap Gap between source bursts
 * @param [in] dst_gap Gap between destination bursts
 * @details Copies data from Global Memory to Cube Buffer with padding mode 1.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_pad1(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad1(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE2 (2201 architecture)
 */
__aicore__ inline void asc_copy_gm2l1_pad2(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad2(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE3 (2201 architecture)
 */
__aicore__ inline void asc_copy_gm2l1_pad3(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad3(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE4 (2201 architecture)
 */
__aicore__ inline void asc_copy_gm2l1_pad4(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad4(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE5 (2201 architecture)
 */
__aicore__ inline void asc_copy_gm2l1_pad5(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad5(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE6 (2201 architecture)
 */
__aicore__ inline void asc_copy_gm2l1_pad6(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad6(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE7 (2201 architecture)
 */
__aicore__ inline void asc_copy_gm2l1_pad7(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad7(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with PAD_MODE8 (2201 architecture)
 */
__aicore__ inline void asc_copy_gm2l1_pad8(__cbuf__ void* dst, __gm__ void* src, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1_pad8(__cbuf__ void* dst, __gm__ void* src, uint32_t size);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1/L0A/L0B buffer with stride (2201 architecture)
 * @brief Supports data types: bfloat16_t, half, float, int32_t, int8_t, uint32_t, uint8_t
 * @param [out] dst Destination buffer in Cube Buffer (L1/L0A/L0B)
 * @param [in] src Source buffer in Global Memory
 * @param [in] base_idx the index id of the fractal matrix in the source matrix
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_gap Destination stride value
 * @details Copies data from Global Memory to Cube Buffer with stride parameters.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_gm2l1(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2l1(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l1(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l1(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l1(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l1(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l1(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_gm2l0a(__ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0a(__ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0a(__ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0a(__ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0a(__ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0a(__ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0a(__ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0a(__ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_gm2l0b(__cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0b(__cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0b(__cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0b(__cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0b(__cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0b(__cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0b(__cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_gm2l0b(__cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride);



/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with ND to NZ format conversion (2201 architecture)
 * @brief Supports data types: int8_t, uint8_t, bfloat16_t, half, int16_t, float, int32_t, uint32_t
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Global Memory
 * @param [in] nd_num Number of ND matrices
 * @param [in] n_value N dimension value
 * @param [in] d_value D dimension value
 * @param [in] src_nd_matrix_stride Source ND matrix stride
 * @param [in] src_d_value Source D value stride
 * @param [in] dst_nz_c0_stride Destination NZ C0 stride
 * @param [in] dst_nz_n_stride Destination NZ N stride
 * @param [in] dst_nz_matrix_stride Destination NZ matrix stride
 * @details Converts data from ND format in Global Memory to NZ format in Cube Buffer.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ half* dst, __gm__ half* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ float* dst, __gm__ float* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num,
    uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride,
    uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);


/*
 * @ingroup CubeDataMove
 * @brief Copy 512B matrices from L1 Buffer to L0A Buffer with high-dimensional split 2D format (2201 architecture)
 * @brief Supports data types: int4b_t, uint8_t, int8_t, half, bfloat16_t, uint32_t, int32_t, float
 * @param [out] dst Destination buffer in Cube L0A Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] start_index Fractal matrix ID, indicates the starting position is the N-th fractal in source operand (0 means the 1st fractal matrix). Range: [0, 65535]. Unit: 512 bytes
 * @param [in] repeat Number of iterations, each iteration can process 512B data. Range: [1, 255]
 * @param [in] src_stride Interval between starting addresses of previous and next fractals in source operand for adjacent iterations. Range: [0, 65535]. Unit: 512 bytes
 * @param [in] dst_gap Interval between adjacent continuous data blocks in destination operand (gap between the tail of previous block and the head of next block). Range: [0, 65535]. Unit: 512 bytes
 * @details Transfers 512B matrices from L1 Buffer to L0A Buffer.
 *          dst starting address must be 512-byte aligned, src starting address must be 32-byte aligned.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0a(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 Buffer to L0A Buffer with high-dimensional split 3D format (2201 architecture)
 * @brief Supports data types: int4b_t, uint8_t, int8_t, half, bfloat16_t, uint32_t, int32_t, float
 * @param [out] dst Destination buffer in Cube L0A Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] k_extension Transfer length in destination operand width dimension. If not covering the rightmost fractal, for half type should be a multiple of 16, for int8_t/uint8_t type should be a multiple of 32. Range: [1, 65535]
 * @param [in] m_extension Transfer length in destination operand height dimension. If not covering the bottommost fractal, for half/int8_t/uint8_t type should be a multiple of 16. Range: [1, 65535]
 * @param [in] k_start_pt Starting point in destination operand width dimension. For half type should be a multiple of 16, for int8_t/uint8_t type should be a multiple of 32. Range: [0, 65535]
 * @param [in] m_start_pt Starting point in destination operand height dimension. If not covering the bottommost fractal, for half/int8_t/uint8_t should be a multiple of 16. Range: [0, 65535]
 * @param [in] stride_w Stride of convolution kernel sliding in source operand width dimension. Range: [1, 63]
 * @param [in] stride_h Stride of convolution kernel sliding in source operand height dimension. Range: [1, 63]
 * @param [in] filter_w Convolution kernel width. Range: [1, 255]
 * @param [in] filter_h Convolution kernel height. Range: [1, 255]
 * @param [in] dilation_filter_w Dilation coefficient of convolution kernel width. Range: [1, 255]
 * @param [in] dilation_filter_h Dilation coefficient of convolution kernel height. Range: [1, 255]
 * @param [in] enable_filter_w_extend Whether to add 256 elements to convolution kernel width on the basis of filter_w. true means increase; false means not increase
 * @param [in] enable_filter_h_extend Whether to add 256 elements to convolution kernel height on the basis of filter_h. true means increase; false means not increase
 * @param [in] enable_transpose Whether to enable enable_transpose function for the entire target matrix, only valid when source operand is half type. true means enable; false means disable
 * @param [in] enable_f_matrix_ctrl Indicates whether asc_copy_l12l0a instruction gets FeatureMap attribute from left matrix or right matrix, currently only supports setting to false
 * @param [in] channel_size Channel size of source operand. Range: [1, 63]. For uint32_t/int32_t/float: 4, N*8, N*8+4. For half/bfloat16: 4, 8, N*16, N*16+4, N*16+8. For int8_t/uint8_t: 4, 8, 16, 32*N, N*32+4, N*32+8, N*32+16. For int4b_t: 8, 16, 32, N*64, N*64+8, N*64+16, N*64+32. N is a positive integer
 * @details Transfers data from L1 Buffer to L0A Buffer using 3D format for convolution operations.
 *          Input feature map and filter format is NC1HWC0, where C0 is the lowest dimension with fixed value 16 (32 for u8/s8 types), C1=C/C0.
 *          dst starting address must be 512-byte aligned, src starting address must be 32-byte aligned.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0a(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_w_extend, bool enable_filter_h_extend , bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);
__aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);
__aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);
__aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);
__aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0b buffer with 2d copy (2201 architecture)
 * @brief Supports data types: int4b_t, uint8_t, int8_t, bfloat16_t, half, int16_t, float, int32_t, uint32_t
 * @param [out] dst Destination buffer in L0B buffer
 * @param [in] src Source buffer in L1B buffer
 * @param [in] start_index Number of ND matrices
 * @param [in] n_value N dimension value
 * @param [in] d_value D dimension value
 * @param [in] src_nd_matrix_stride Source ND matrix stride
 * @param [in] src_d_value Source D value stride
 * @param [in] dst_nz_c0_stride Destination NZ C0 stride
 * @param [in] dst_nz_n_stride Destination NZ N stride
 * @param [in] dst_nz_matrix_stride Destination NZ matrix stride
 * @details Converts data from ND format in Global Memory to NZ format in Cube Buffer.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0b(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);


/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 Buffer to L0B Buffer with high-dimensional split 3D format (2201 architecture)
 * @brief Supports data types: int4b_t, uint8_t, int8_t, half, bfloat16_t, uint32_t, int32_t, float
 * @param [out] dst Destination buffer in Cube L0B Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] k_extension Transfer length in destination operand width dimension. If not covering the rightmost fractal, for half type should be a multiple of 16, for int8_t/uint8_t type should be a multiple of 32. Range: [1, 65535]
 * @param [in] m_extension Transfer length in destination operand height dimension. If not covering the bottommost fractal, for half/int8_t/uint8_t type should be a multiple of 16. Range: [1, 65535]
 * @param [in] k_start_pt Starting point in destination operand width dimension. For half type should be a multiple of 16, for int8_t/uint8_t type should be a multiple of 32. Range: [0, 65535]
 * @param [in] m_start_pt Starting point in destination operand height dimension. If not covering the bottommost fractal, for half/int8_t/uint8_t should be a multiple of 16. Range: [0, 65535]
 * @param [in] stride_w Stride of convolution kernel sliding in source operand width dimension. Range: [1, 63]
 * @param [in] stride_h Stride of convolution kernel sliding in source operand height dimension. Range: [1, 63]
 * @param [in] filter_w Convolution kernel width. Range: [1, 255]
 * @param [in] filter_h Convolution kernel height. Range: [1, 255]
 * @param [in] dilation_filter_w Dilation coefficient of convolution kernel width. Range: [1, 255]
 * @param [in] dilation_filter_h Dilation coefficient of convolution kernel height. Range: [1, 255]
 * @param [in] enable_filter_size_w Whether to add 256 elements to convolution kernel width on the basis of filter_w. true means increase; false means not increase
 * @param [in] enable_filter_size_h Whether to add 256 elements to convolution kernel height on the basis of filter_h. true means increase; false means not increase
 * @param [in] enable_transpose Whether to enable enable_transpose function for the entire target matrix, only valid when source operand is half type. true means enable; false means disable
 * @param [in] enable_f_matrix_ctrl Indicates whether asc_copy_l12l0b instruction gets FeatureMap attribute from left matrix or right matrix, currently only supports setting to false
 * @param [in] channel_size Channel size of source operand. Range: [1, 63]. For uint32_t/int32_t/float: 4, N*8, N*8+4. For half/bfloat16: 4, 8, N*16, N*16+4, N*16+8. For int8_t/uint8_t: 4, 8, 16, 32*N, N*32+4, N*32+8, N*32+16. For int4b_t: 8, 16, 32, N*64, N*64+8, N*64+16, N*64+32. N is a positive integer
 * @details Transfers data from L1 Buffer to L0A Buffer using 3D format for convolution operations.
 *          Input feature map and filter format is NC1HWC0, where C0 is the lowest dimension with fixed value 16 (32 for u8/s8 types), C1=C/C0.
 *          dst starting address must be 512-byte aligned, src starting address must be 32-byte aligned.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, 
    uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, 
    bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt,
    uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, 
    bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt,
    uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h,
    bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt,
    uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h,
    bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt,
    uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h,
    bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

/*
 * @ingroup CubeDataMove
 * @brief Set boundary value for 3D format data copy interfaces in L1 Buffer
 * @param [in] value Boundary value for asc_copy_l12l0a/asc_copy_l12l0b 3D format copy interfaces in L1 Buffer, in bytes. Non-zero boundary value must not be less than 1024. If set to 0, means no boundary and asc_copy_l12l0a/asc_copy_l12l0b can use the entire L1 Buffer
 * @details Sets the boundary value for 3D format copy operations from L1 Buffer.
 *          Must be used together with asc_copy_l12l0a/asc_copy_l12l0b 3D format copy interfaces.
 *          If boundary value is set, the L1 Buffer initial address of asc_copy_l12l0a/asc_copy_l12l0b must be within the set boundary (value).
 *          If the source operand address of asc_copy_l12l0a/asc_copy_l12l0b exceeds the set boundary value, data will be read from the starting address of L1 Buffer.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_set_l13d_size(uint64_t value);

/*
 * @ingroup CubeDataMove
 * @brief Load 2D format data from L1 Buffer to L0A Buffer with enable_transpose operation
 * @brief Supports data types: half, bfloat16_t, float, int32_t, int8_t, uint32_t, uint8_t
 * @param [out] dst Destination buffer in Cube L0A Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] index_id Square matrix ID, indicates starting position in source operand
 * @param [in] repeat Iteration repeat count
 * @param [in] src_stride Interval between starting addresses of adjacent square matrices in source operand
 * @param [in] dst_stride Interval from end address of first fractal in previous iteration to start address of first fractal in next iteration
 * @param [in] enable_addr_decrement Address update mode. false: increment; true: decrement
 * @param [in] dst_frac_stride Interval between adjacent fractals within each iteration in destination operand, unit is 512B
 * @details Implements enable_transpose 2D format data loading from L1 Buffer to L0A Buffer.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_trans(__ca__ half* dst, __cbuf__ half* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0a_trans(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0a_trans(__ca__ float* dst, __cbuf__ float* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0a_trans(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0a_trans(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0a_trans(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0a_trans(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

/*
 * @ingroup CubeDataMove
 * @brief Load 2D format data from L1 Buffer to L0B Buffer with enable_transpose operation
 * @brief Supports data types: half, bfloat16_t, float, int32_t, int8_t, uint32_t, uint8_t
 * @param [out] dst Destination buffer in Cube L0B Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] index_id Square matrix ID, indicates starting position in source operand
 * @param [in] repeat Iteration repeat count
 * @param [in] src_stride Interval between starting addresses of adjacent square matrices in source operand
 * @param [in] dst_stride Interval from end address of first fractal in previous iteration to start address of first fractal in next iteration
 * @param [in] enable_addr_decrement Address update mode. false: increment; true: decrement
 * @param [in] dst_frac_stride Interval between adjacent fractals within each iteration in destination operand, unit is 512B
 * @details Implements enable_transpose 2D format data loading from L1 Buffer to L0B Buffer.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ float* dst, __cbuf__ float* src, uint16_t index_id, 
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);


__aicore__ inline void asc_copy_l12l0b_trans(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id,
    uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool enable_addr_decrement, uint16_t dst_frac_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy 512B dense weight matrix from L1 Buffer to L0B Buffer with sparsification using 128B index matrix
 * @param [out] dst Destination buffer in L0B Buffer
 * @param [in] src Source buffer in L1 Buffer
 * @param [in] index Index matrix buffer in L1 Buffer
 * @param [in] start_index Fractal matrix ID, indicates starting position in source operand
 * @param [in] repeat Iteration count, each iteration processes 512B data
 * @details Transfers dense weight matrix from L1 to L0B while using index matrix for sparsification.
 *          Index matrix is stored in reverse order in int8_t address.
 *          dst and src starting addresses must be 32-byte aligned.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_sparse(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat);

/*
 * @ingroup CubeDataMove
 * @brief Load image data from Global Memory to L1 Buffer
 * @brief Supports data types: int8_t, half
 * @param [out] dst Destination buffer in L1 Buffer
 * @param [in] hor_size Horizontal width of loaded image from source, in pixels
 * @param [in] ver_size Vertical height of loaded image from source, in pixels
 * @param [in] hor_start_pos Horizontal starting position of loaded image in source image, in pixels
 * @param [in] ver_start_pos Vertical starting position of loaded image in source image, in pixels
 * @param [in] src_hor_size Source image horizontal width, in pixels
 * @param [in] top_pad_size Padding pixels at top of destination image
 * @param [in] bot_pad_size Padding pixels at bottom of destination image
 * @param [in] left_pad_size Padding pixels at left of destination image
 * @param [in] right_pad_size Padding pixels at right of destination image
 * @details Transfers image data from Global Memory to L1 Buffer.
 *          dst starting address must be 32-byte aligned.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_load_image_to_cbuf(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

__aicore__ inline void asc_load_image_to_cbuf(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

/*
 * @ingroup CubeDataMove
 * @brief Set scalar quantization parameters for quantization during data copy process
 * @param [in] config Scalar quantization parameters used during quantization process
 * @details Sets quantization parameters when performing inline quantization during data copy.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config);

/*
 * @ingroup CubeDataMove
 * @brief Set format conversion parameters for inline NZ to ND format conversion during data copy
 * @param [in] nd_num Number of ND matrices
 * @param [in] src_nd_stride Source stride in fractal size units, offset between starting addresses of adjacent NZ matrices
 * @param [in] dst_nd_stride Destination stride, offset between starting addresses of adjacent ND matrices in elements
 * @details Sets configuration for NZ to ND format conversion during DataCopy (CO1->GM, CO1->A1).
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_set_l0c_copy_params(uint16_t nd_num, uint16_t src_nd_stride, uint16_t dst_nd_stride);


#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with padding function mode (3510 architecture)
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Global Memory
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] pad_func_mode Padding function mode
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 */
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t burst_count, uint32_t burst_len, uint8_t pad_func_mode,
                                     uint64_t src_stride, uint32_t dst_stride);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with decomposition mode for various data types (3510 architecture)
 * @brief Supports data types: bfloat16_t, float, float8_e4m3_t, float8_e5m2_t, half, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, float4_e1m2x2_t, float4_e2m1x2_t, void
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] m_start_position M dimension starting position (uint32_t)
 * @param [in] k_start_position K dimension starting position (uint32_t)
 * @param [in] dst_stride Destination stride value
 * @param [in] m_step M dimension step value
 * @param [in] k_step K dimension step value
 * @param [in] decomp_mode Decomposition mode setting
 * @param [in] l2_cache_ctrl L2 cache control value
 * @details Copies data from Global Memory (__gm__) to Cube Buffer (__cbuf__) with decomposition mode for 3510 architecture.
 *          Supports bfloat16_t, float, float8_e4m3_t, float8_e5m2_t, half, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, float4_e1m2x2_t, float4_e2m1x2_t, void data types.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_gm2l1(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ float8_e4m3_t* dst, __gm__ float8_e4m3_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ float8_e5m2_t* dst, __gm__ float8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ float4_e1m2x2_t* dst, __gm__ float4_e1m2x2_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ float4_e2m1x2_t* dst, __gm__ float4_e2m1x2_t* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position,
                                         uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

/*
 * @ingroup CubeDataMove
 * @brief Set loop size parameters for GM to L1 copy operations (3510 architecture)
 * @param [in] loop1_size Loop 1 size value
 * @param [in] loop2_size Loop 2 size value
 * @details Sets loop size parameters for multi-dimensional data transfer from Global Memory to L1 buffer.
 */
__aicore__ inline void asc_set_gm2l1_loop_size(uint64_t loop1_size, uint64_t loop2_size);

/*
 * @ingroup CubeDataMove
 * @brief Set loop 1 stride parameters for GM to L1 copy (3510 architecture)
 * @param [in] loop1_src_stride Loop 1 source stride value
 * @param [in] loop1_dst_stride Loop 1 destination stride value
 */
__aicore__ inline void asc_set_gm2l1_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Set loop 2 stride parameters for GM to L1 copy (3510 architecture)
 * @param [in] loop2_src_stride Loop 2 source stride value
 * @param [in] loop2_dst_stride Loop 2 destination stride value
 */
__aicore__ inline void asc_set_gm2l1_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Set padding value for GM to L1 copy (3510 architecture)
 * @param [in] pad_val Padding value to use
 */
__aicore__ inline void asc_set_gm2l1_pad(uint32_t pad_val);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with alignment padding (3510 architecture)
 * @brief Supports data types: bfloat16_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Global Memory
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] left_padding_count Left padding count
 * @param [in] right_padding_count Right padding count
 * @param [in] enable_data_select Data select bit flag
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] burst_src_stride Burst source stride
 * @param [in] burst_dst_stride Burst destination stride
 * @details Copies data from Global Memory (__gm__) to Cube Buffer (__cbuf__) with alignment padding.
 */
__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                       uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                      uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                      uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                            uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                            uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                            uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                            uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                            uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                            uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
                                             uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with DN to NZ format conversion (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Global Memory
 * @param [in] loop1_src_stride Loop 1 source stride
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] n_value N dimension value
 * @param [in] d_value D dimension value
 * @param [in] loop4_src_stride Loop 4 source stride
 * @param [in] enable_smallc0 Small C0 enable flag
 * @details Converts data from DN format in Global Memory to NZ format in Cube Buffer.
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                             uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ float8_e4m3_t* dst, __gm__ float8_e4m3_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ float8_e5m2_t* dst, __gm__ float8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                             uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L1 buffer with ND to NZ format conversion (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t
 * @details Converts data from ND format in Global Memory to NZ format in Cube Buffer.
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                             uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ float8_e4m3_t* dst, __gm__ float8_e4m3_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ float8_e5m2_t* dst, __gm__ float8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);


__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                            uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value,
                                             uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

/*
 * @ingroup CubeDataMove
 * @brief Set MTE2_NZ_PARA register value before calling asc_copy_gm2l1_dn2nz and asc_copy_gm2l1_nd2nz
 * @param [in] config Register value to set. Bit 15:0 is ND/DN matrix count, bit 31:16 is N dimension block interval, bit 47:32 is C0 column interval in same NZ matrix, bit 63:48 is NZ matrix interval between ND/DN matrices
 * @details Sets parameters for format conversion in MTE2_NZ_PARA register before ND/DN to NZ format conversion.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to Global Memory (3510 architecture)
 * @param [out] dst Destination buffer in Global Memory
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies data from Cube Buffer (__cbuf__) to Global Memory (__gm__).
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12gm(__gm__ void* dst, __cbuf__ void* src, uint16_t burst_count, uint16_t burst_len,
                                     uint16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0A buffer with M/K position (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t
 * @param [out] dst Destination buffer in Cube L0A Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] m_start_position M dimension starting position
 * @param [in] k_start_position K dimension starting position
 * @param [in] m_step M dimension step value
 * @param [in] k_step K dimension step value
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies data from Cube Buffer (L1) to Cube L0A Buffer with M/K dimension parameters.
 *          This is a hardware-level instruction that requires manual synchronization.
 */
__aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy MX scale data from L1 buffer to L0A buffer (3510 architecture)
 * @param [in] dst Destination address in L0A buffer (uint64_t)
 * @param [in] src Source buffer pointer in L1 buffer (fp8_e8m0_t*)
 * @param [in] x_start_pos X dimension starting position
 * @param [in] y_start_pos Y dimension starting position
 * @param [in] x_step X dimension step value
 * @param [in] y_step Y dimension step value
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies MX scale data from Cube Buffer (L1) to Cube L0A Buffer for mixed precision operations.
 */
__aicore__ inline void asc_copy_l12l0a_mx(uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos,
    uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy MX scale data from L1 buffer to L0B buffer (3510 architecture)
 */
__aicore__ inline void asc_copy_l12l0b_mx(uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos,
    uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0A buffer with enable_transpose (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t
 * @details Transposes data while copying from Cube Buffer (L1) to Cube L0A Buffer.
 */
__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose(__ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0B buffer with M/K position (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t
 * @param [out] dst Destination buffer in Cube L0B Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] m_start_position M dimension starting position
 * @param [in] k_start_position K dimension starting position
 * @param [in] m_step M dimension step value
 * @param [in] k_step K dimension step value
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies data from Cube Buffer (L1) to Cube L0B Buffer with M/K dimension parameters.
 */
__aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0B buffer with enable_transpose (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t
 * @details Transposes data while copying from Cube Buffer (L1) to Cube L0B Buffer.
 */
__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0B buffer with transposed addressing (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int32_t, int8_t, uint32_t, uint8_t, float4_e1m2x2_t, float4_e2m1x2_t, int4b_t
 * @param [out] dst Destination buffer in Cube L0B Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] index_id Index identifier
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @param [in] dst_frac_stride Destination fractional stride
 * @param [in] src_frac_stride Source fractional stride
 * @details Copies data from Cube Buffer (L1) to Cube L0B Buffer with transposed addressing mode.
 */
__aicore__ inline void asc_copy_l12l0b_trans(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ float8_e4m3_t* dst, __cbuf__ float8_e4m3_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ float8_e5m2_t* dst, __cbuf__ float8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ float4_e1m2x2_t* dst, __cbuf__ float4_e1m2x2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ float4_e2m1x2_t* dst, __cbuf__ float4_e2m1x2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

__aicore__ inline void asc_copy_l12l0b_trans(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_stride, uint16_t dst_frac_stride, uint16_t src_frac_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0C buffer (3510 architecture)
 * @brief Supports dst/src types: bfloat16_t, half, float, int32_t, uint32_t; supports src: float
 * @param [out] dst Destination buffer in Cube L0C Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies data from Cube Buffer (__cbuf__) to Cube L0C Buffer (__cc__).
 */
__aicore__ inline void asc_copy_l12l0c(__cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t burst_count, uint16_t burst_len,
                                        uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0c(__cc__ half* dst, __cbuf__ half* src, uint16_t burst_count, uint16_t burst_len,
                                        uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0c(__cc__ half* dst, __cbuf__ float* src, uint16_t burst_count, uint16_t burst_len,
                                        uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0c(__cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t burst_count, uint16_t burst_len,
                                        uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0c(__cc__ float* dst, __cbuf__ float* src, uint16_t burst_count, uint16_t burst_len,
                                        uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0c(__cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t burst_count, uint16_t burst_len,
                                        uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0c(__cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t burst_count, uint16_t burst_len,
  	                                    uint16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to Unified buffer (3510 architecture)
 */
__aicore__ inline void asc_copy_l12ub(__ubuf__ void * dst, __cbuf__ void * src, bool sub_blockid,
                                      uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to BT buffer (3510 architecture)
 * @brief Supports data types: bfloat16_t, half, float, int32_t
 * @param [out] dst Destination address in BT buffer (uint64_t)
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] conv_control Conversion control parameter
 * @param [in] burst_count Number of burst transfers
 * @param [in] burst_len Length of each burst
 * @param [in] source_gap Gap between source data blocks
 * @param [in] dst_gap Gap between destination data blocks
 */
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint16_t conv_control, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t source_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint16_t conv_control, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t source_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint32_t size);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint16_t conv_control, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t source_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint32_t size);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint16_t conv_control, uint16_t burst_count,
                                      uint16_t burst_len, uint16_t source_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint32_t size);

/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to Feature buffer version 2 (3510 architecture)
 */
__aicore__ inline void asc_copy_l12fb_v2(__fbuf__ void * dst, __cbuf__ void * src, uint16_t burst_count,
                                             uint16_t burst_len, uint16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12fb_v2(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size);


/**
 * @ingroup CubeDataMove
 * @brief Copy data from L1 buffer to L0A buffer with convolution parameters (3510 architecture)
 * @brief Supports data types: bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int32_t, int8_t, uint32_t, uint8_t, int16_t, uint16_t
 * @param [out] dst Destination buffer in Cube L0A Buffer
 * @param [in] src Source buffer in Cube Buffer (L1)
 * @param [in] k_extension K dimension extension value
 * @param [in] m_extension M dimension extension value
 * @param [in] k_start_pt K dimension starting point
 * @param [in] m_start_pt M dimension starting point
 * @param [in] stride_w Width stride value
 * @param [in] stride_h Height stride value
 * @param [in] filter_w Filter width
 * @param [in] filter_h Filter height
 * @param [in] dilation_filter_w Filter width dilation
 * @param [in] dilation_filter_h Filter height dilation
 * @param [in] enable_filter_size_w Filter size width flag
 * @param [in] enable_filter_size_h Filter size height flag
 * @param [in] enable_transpose enable_transpose enable flag
 * @param [in] fmatrix_ctrl Feature matrix control flag
 * @param [in] channel_size Channel size value
 * @details Copies data from Cube Buffer (L1) to Cube L0A Buffer for convolution operations.
 */
__aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int16_t* dst, __cbuf__ int16_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ uint16_t* dst, __cbuf__ uint16_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ int4b_t* dst, __cbuf__ int4b_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0a(__ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ int16_t* dst, __cbuf__ int16_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ uint16_t* dst, __cbuf__ uint16_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ int4b_t* dst, __cbuf__ int4b_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);
__aicore__ inline void asc_copy_l12l0b(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src,
    uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to L1 buffer with quantization and activation (3510 architecture)
 * @brief Supports dst types: half, int8_t, uint8_t, float, int32_t; src types: float, int32_t
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Cube L0C Buffer
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Destination stride value
 * @param [in] src_stride Source stride value
 * @param [in] clip_relu_pre_mode_mode_mode Clip/ReLU pre-operation mode
 * @param [in] unit_flag Unit flag setting
 * @param [in] quant_pre_mode_mode Pre-quantization parameter
 * @param [in] relu_pre_mode_mode Pre-ReLU parameter
 * @param [in] enable_channel_split Channel split enable flag
 * @param [in] enable_nz2nd NZ to ND conversion enable flag
 * @param [in] quant_post_mode_mode Post-quantization parameter
 * @param [in] relu_post_mode_mode Post-ReLU parameter
 * @param [in] enable_clip_relu_post_mode_mode Clip/ReLU post-operation enable flag
 * @param [in] eltwise_op Element-wise operation type
 * @param [in] enable_antiquant Element-wise anti-quantization enable flag
 * @param [in] enable_c0_pad C0 padding enable flag
 * @details Copies data from Cube L0C Buffer (__cc__) to Cube Buffer (__cbuf__) with quantization and activation operations.
 *          Supports quantization, ReLU, element-wise operations and format conversion.
 */
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t clip_relu_pre_mode,
    uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode,
    uint8_t eltwise_op, uint8_t eltwise_antq_cfg, bool enable_c0_pad);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to L1 buffer with extended parameters (3510 architecture)
 * @param [out] dst Destination buffer in Cube Buffer (L1)
 * @param [in] src Source buffer in Cube L0C Buffer
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Destination stride value
 * @param [in] src_stride Source stride value
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] clip_relu_pre_mode_mode_mode Clip/ReLU pre-operation mode
 * @param [in] unit_flag Unit flag setting
 * @param [in] quant_pre_mode_mode Pre-quantization parameter
 * @param [in] relu_pre_mode_mode Pre-ReLU parameter
 * @param [in] enable_channel_split Channel split enable flag
 * @param [in] enable_nz2nd NZ to ND conversion enable flag
 * @param [in] quant_post_mode_mode Post-quantization parameter
 * @param [in] relu_post_mode_mode Post-ReLU parameter
 * @param [in] enable_clip_relu_post_mode_mode Clip/ReLU post-operation enable flag
 * @param [in] eltwise_op Element-wise operation type
 * @param [in] enable_antiquant Element-wise anti-quantization enable flag
 * @param [in] enable_c0_pad C0 padding enable flag
 * @param [in] enable_broadcast Broadcast enable flag
 * @param [in] enable_nz2dn NZ to DN conversion enable flag
 */
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ void *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, 
    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2l1(__cbuf__ void *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, 
    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post_mode,
    uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to Global Memory with quantization and activation (3510 architecture)
 * @brief Supports dst types: bfloat16_t, half, float8_e4m3_t, float8_e5m2_t, hifloat8_t, int8_t, uint8_t, float, int32_t, void; src types: float, int32_t
 * @param [out] dst Destination buffer in Global Memory
 * @param [in] src Source buffer in Cube L0C Buffer
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Loop destination stride value
 * @param [in] src_stride Loop source stride value
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] clip_relu_pre_mode_mode_mode Clip/ReLU pre-operation mode
 * @param [in] unit_flag Unit flag control setting
 * @param [in] quant_pre_mode_mode Pre-quantization parameter
 * @param [in] relu_pre_mode_mode Pre-ReLU parameter
 * @param [in] enable_split Split enable flag
 * @param [in] enable_nz2nd NZ to ND conversion enable flag
 * @param [in] quant_post_mode_mode Post-quantization parameter
 * @param [in] relu_post_mode_mode Post-ReLU parameter
 * @param [in] enable_clip_relu_post_mode_mode Clip/ReLU post-operation enable flag
 * @param [in] eltwise_op Element-wise operation type
 * @param [in] enable_antiquant Element-wise anti-quantization enable flag
 * @param [in] enable_c0_pad C0 padding enable flag
 * @param [in] enable_broadcast Broadcast enable flag
 * @param [in] enable_nz2dn NZ to DN conversion enable flag
 * @details Copies data from Cube L0C Buffer (__cc__) to Global Memory (__gm__) with quantization and activation operations.
 */
__aicore__ inline void asc_copy_l0c2gm(__gm__ bfloat16_t *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ half *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ fp8_e4m3fn_t *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ fp8_e5m2_t *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ hifloat8_t *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ int8_t *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ uint8_t *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ float *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ bfloat16_t *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ half *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ fp8_e4m3fn_t *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ fp8_e5m2_t *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ hifloat8_t *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ int8_t *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ uint8_t *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ int32_t *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ void *dst, __cc__ float *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm(__gm__ void *dst, __cc__ int32_t *src,
                    uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl,
                    uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split,
                    bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op,
                    bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);


/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to Unified buffer with quantization and activation (3510 architecture)
 * @brief Supports dst types: bfloat16_t, half, float8_e4m3_t, float8_e5m2_t, hifloat8_t, int8_t, uint8_t, float, int32_t, void; src types: float, int32_t
 * @param [out] dst Destination buffer in Unified Buffer
 * @param [in] src Source buffer in Cube L0C Buffer
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Loop destination stride value
 * @param [in] src_stride Loop source stride value
 * @param [in] dual_dst_ctl Dual destination control
 * @param [in] sub_blockid Sub-block ID
 * @param [in] clip_relu_pre_mode_mode_mode Clip/ReLU pre-operation mode
 * @param [in] unit_flag Unit flag control setting
 * @param [in] quant_pre_mode_mode Pre-quantization parameter
 * @param [in] relu_pre_mode_mode Pre-ReLU parameter
 * @param [in] enable_split Split enable flag
 * @param [in] enable_nz2nd NZ to ND conversion enable flag
 * @param [in] quant_post_mode_mode Post-quantization parameter
 * @param [in] relu_post_mode_mode Post-ReLU parameter
 * @param [in] enable_clip_relu_post_mode_mode Clip/ReLU post-operation enable flag
 * @param [in] eltwise_op Element-wise operation type
 * @param [in] enable_antiquant Element-wise anti-quantization enable flag
 * @param [in] enable_c0_pad C0 padding enable flag
 * @param [in] enable_broadcast Broadcast enable flag
 * @param [in] enable_nz2dn NZ to DN conversion enable flag
 * @details Copies data from Cube L0C Buffer (__cc__) to Unified Buffer (__ubuf__) with quantization and activation operations.
 */
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ bfloat16_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ half *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ fp8_e4m3fn_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ fp8_e5m2_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ hifloat8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ int8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ uint8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ float *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ bfloat16_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ half *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ fp8_e4m3fn_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ fp8_e5m2_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ hifloat8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ int8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ uint8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ int32_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ void *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ void *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size,
                            uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode,
                            uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode,
                            uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant,
                            bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif    
