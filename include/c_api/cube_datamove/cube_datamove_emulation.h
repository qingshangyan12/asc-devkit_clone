/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/**
 * @file cube_datamove_emulation.h
 * @brief Software emulation implementation of cube data movement operations.
 * @details This file provides software emulation of hardware cube data movement instructions.
 *          The purpose is to enhance operator development usability and simplify programming.
 *          Since this is a software emulation layer (not direct hardware instruction execution),
 *          these interfaces provide simplified synchronous (_sync) versions for ease of use.
 *          
 *          These interfaces support cube data movement operations including:
 *          - Data copy operations: copy_l12fb, copy_l12bt, copy_gm2l1, copy_l12l0a, copy_l12l0b, etc.
 *          - Data fill operations: fill_l0a, fill_l0b, fill_l1, etc.
 *          - Format conversion: nd2nz, enable_transpose, etc.
 *          - Image loading: load_image_to_cbuf, etc.
 *          - Matrix operations: copy_l0c2gm, copy_l0c2ub, etc.
 *          
 *          Supported data types: half, float, int8_t, int16_t, int32_t, uint8_t, uint16_t, uint32_t,
 *          bfloat16_t, int4b_t, fp8_e8m0_t, fp4x2_e2m1_t, fp4x2_e1m2_t, etc.
 * 
 * @note Usage Note: These emulated interfaces are designed for development convenience and
 *       provide automatic synchronization. The _sync suffix indicates that these interfaces
 *       wait for operation completion before returning, making them easier to use than
 *       hardware-native asynchronous interfaces.
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
 * @brief Copy data from Cube Buffer (L1) to Feature Buffer (L1) for various data types
 * @param [out] dst Destination buffer pointer in Feature Buffer (L1)
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] size Data size in bytes to copy
 * @details Copies data from Cube Buffer (__cbuf__) to Feature Buffer (__fbuf__) in L1 memory.
 *          This is a software emulation interface with automatic synchronization.
 *          The _sync suffix indicates that this interface waits for operation completion before returning.
 */
__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void *dst, __cbuf__ void *src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to BT buffer for various data types
 * @param [out] dst Destination buffer address (uint64_t)
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] size Data size in bytes to copy
 * @details Copies data from Cube Buffer (__cbuf__) to BT buffer.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void *src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Fill L0A buffer with scalar value for various data types
 * @param [out] dst Destination buffer pointer in L0A
 * @param [in] value Scalar value to fill (half or uint32_t)
 * @param [in] config Fill configuration parameters
 * @details Fills L0A buffer with specified scalar value.
 *          Supports half, float, int16_t, int32_t, uint16_t, uint32_t, bfloat16_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ half *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ half *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ float *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ float *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_sync(__ca__ bfloat16_t *dst, bfloat16_t value, const asc_fill_value_config& config);

/*
 * @ingroup CubeDataMove
 * @brief Fill L0B buffer with scalar value for various data types
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] value Scalar value to fill (half or uint32_t)
 * @param [in] config Fill configuration parameters
 * @details Fills L0B buffer with specified scalar value.
 *          Supports half, float, int16_t, int32_t, uint16_t, uint32_t, bfloat16_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ half *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ half *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ float *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ float *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t *dst, bfloat16_t value, const asc_fill_value_config& config);

/*
 * @ingroup CubeDataMove
 * @brief Fill L1 Cube Buffer with scalar value for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] value Scalar value to fill (half, bfloat16_t or uint32_t)
 * @param [in] config Fill configuration parameters
 * @details Fills L1 Cube Buffer (__cbuf__) with specified scalar value.
 *          Supports half, float, int16_t, int32_t, uint16_t, uint32_t, bfloat16_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ half *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ float *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ float *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t *dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t *dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t *dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t *dst, uint32_t value, const asc_fill_value_config& config);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] size Data size in bytes to copy
 * @details Copies data from Global Memory (__gm__) to Cube Buffer (__cbuf__) in L1.
 *          Supports multiple data types with void pointer interface.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with padding support
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] size Data size in bytes to copy
 * @details Copies data from Global Memory (__gm__) to Cube Buffer (__cbuf__) with padding.
 *          Supports pad1 to pad8 modes for different padding requirements.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_pad1_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad2_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad3_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad4_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad5_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad6_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad7_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

__aicore__ inline void asc_copy_gm2l1_pad8_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with stride parameters for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] base_idx Base index for starting position
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies data from Global Memory (__gm__) to Cube Buffer (__cbuf__) with stride control.
 *          Supports bfloat16_t, half, float, int32_t, int8_t, uint32_t, uint8_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ half *dst, __gm__ half *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float *dst, __gm__ float *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with ND to NZ format conversion for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1) with NZ format
 * @param [in] src Source buffer pointer in Global Memory with ND format
 * @param [in] nd_num Number of ND matrices
 * @param [in] n_value N dimension value
 * @param [in] d_value D dimension value
 * @param [in] src_nd_matrix_stride Source ND matrix stride
 * @param [in] src_d_value Source D value stride
 * @param [in] dst_nz_c0_stride Destination NZ C0 stride
 * @param [in] dst_nz_n_stride Destination NZ N stride
 * @param [in] dst_nz_matrix_stride Destination NZ matrix stride
 * @details Converts ND format data from Global Memory (__gm__) to NZ format in Cube Buffer (__cbuf__).
 *          Supports int8_t, uint8_t, bfloat16_t, half, int16_t, float, int32_t, uint32_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ half *dst, __gm__ half *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ int16_t *dst, __gm__ int16_t *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ float *dst, __gm__ float *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint16_t nd_num, uint16_t n_value, uint16_t d_value, uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride, uint16_t dst_nz_matrix_stride);


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
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ half *dst, __cbuf__ half *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float *dst, __cbuf__ float *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);


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
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_sync(__cb__ half *dst, __cbuf__ half *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float *dst, __cbuf__ float *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

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
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ half *dst, __cbuf__ half *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t *dst, __cbuf__ int32_t *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float *dst, __cbuf__ float *src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

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
 * @param [in] enable_filter_size_w Whether to add 256 elements to convolution kernel width on the basis of filter_w. true means increase; false means not increase
 * @param [in] enable_filter_size_h Whether to add 256 elements to convolution kernel height on the basis of filter_h. true means increase; false means not increase
 * @param [in] enable_transpose Whether to enable enable_transpose function for the entire target matrix, only valid when source operand is half type. true means enable; false means disable
 * @param [in] enable_f_matrix_ctrl Indicates whether asc_copy_l12l0a instruction gets FeatureMap attribute from left matrix or right matrix, currently only supports setting to false
 * @param [in] channel_size Channel size of source operand. Range: [1, 63]. For uint32_t/int32_t/float: 4, N*8, N*8+4. For half/bfloat16: 4, 8, N*16, N*16+4, N*16+8. For int8_t/uint8_t: 4, 8, 16, 32*N, N*32+4, N*32+8, N*32+16. For int4b_t: 8, 16, 32, N*64, N*64+8, N*64+16, N*64+32. N is a positive integer
 * @details Transfers data from L1 Buffer to L0A Buffer using 3D format for convolution operations.
 *          Input feature map and filter format is NC1HWC0, where C0 is the lowest dimension with fixed value 16 (32 for u8/s8 types), C1=C/C0.
 *          dst starting address must be 512-byte aligned, src starting address must be 32-byte aligned.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ half *dst, __cbuf__ half *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t *dst, __cbuf__ int32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float *dst, __cbuf__ float *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool enable_f_matrix_ctrl, uint16_t channel_size);

/*
 * @ingroup CubeDataMove
 * @brief Copy sparse data from Cube Buffer (L1) to L0B buffer
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] index Index buffer pointer in Cube Buffer (L1) for sparse data
 * @param [in] start_index Starting index position
 * @param [in] repeat Number of repeat operations
 * @details Copies sparse data from Cube Buffer (__cbuf__) to L0B buffer (__cb__) using index information.
 *          Supports int8_t data type for sparse matrix operations.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_sparse_sync(__cb__ int8_t *dst, __cbuf__ int8_t *src, __cbuf__ int8_t *index, uint16_t start_index, uint8_t repeat);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0A buffer with enable_transpose operation for various data types
 * @param [out] dst Destination buffer pointer in L0A
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] index_id Index identifier
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_gap Destination stride value
 * @param [in] enable_addr_decrement Address mode flag
 * @param [in] dst_frac_gap Destination fractional stride
 * @details Transposes and copies data from Cube Buffer (__cbuf__) to L0A buffer (__ca__).
 *          Supports half, bfloat16_t, float, int32_t, int8_t, uint32_t, uint8_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ half *dst, __cbuf__ half *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ float *dst, __cbuf__ float *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ int32_t *dst, __cbuf__ int32_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0a_trans_sync(__ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0B buffer with enable_transpose operation for various data types
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] index_id Index identifier
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_gap Destination stride value
 * @param [in] enable_addr_decrement Address mode flag
 * @param [in] dst_frac_gap Destination fractional stride
 * @details Transposes and copies data from Cube Buffer (__cbuf__) to L0B buffer (__cb__).
 *          Supports bfloat16_t, half, float, int32_t, int8_t, uint32_t, uint8_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint64_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ half *dst, __cbuf__ half *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ float *dst, __cbuf__ float *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint64_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint64_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint64_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint64_t dst_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, bool enable_addr_decrement, uint64_t dst_frac_gap);

/*
 * @ingroup CubeDataMove
 * @brief Load image data to Cube Buffer (L1) with padding support for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] hor_size Horizontal size of the image region
 * @param [in] ver_size Vertical size of the image region
 * @param [in] hor_start_pos Horizontal starting position
 * @param [in] ver_start_pos Vertical starting position
 * @param [in] src_hor_size Source horizontal size
 * @param [in] top_pad_size Top padding size
 * @param [in] bot_pad_size Bottom padding size
 * @param [in] left_pad_size Left padding size
 * @param [in] right_pad_size Right padding size
 * @details Loads image data with padding from external memory to Cube Buffer (__cbuf__).
 *          Supports half and int8_t data types for image processing operations.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ half *dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

__aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ int8_t *dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L0B buffer for various data types
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] base_idx Base index for starting position
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_gap Destination stride value
 * @details Copies data from Global Memory (__gm__) to L0B buffer (__cb__).
 *          Supports bfloat16_t, half, float, int32_t, int8_t, uint32_t, uint8_t, int4b_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ half *dst, __gm__ half *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ float *dst, __gm__ float *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ int32_t *dst, __gm__ int32_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ int8_t *dst, __gm__ int8_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ uint32_t *dst, __gm__ uint32_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ uint8_t *dst, __gm__ uint8_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0b_sync(__cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to L0A buffer for various data types
 * @param [out] dst Destination buffer pointer in L0A
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] base_idx Base index for starting position
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_gap Destination stride value
 * @details Copies data from Global Memory (__gm__) to L0A buffer (__ca__).
 *          Supports bfloat16_t, half, float, int32_t, uint8_t, int8_t, uint32_t, int4b_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ half *dst, __gm__ half *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ float *dst, __gm__ float *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int32_t *dst, __gm__ int32_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ uint8_t *dst, __gm__ uint8_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int8_t *dst, __gm__ int8_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ uint32_t *dst, __gm__ uint32_t *src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to Cube Buffer (L1) for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in L0C
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Destination stride value
 * @param [in] src_stride Source stride value
 * @param [in] unit_flag Unit flag mode setting
 * @param [in] quant_pre_mode Pre-quantization parameter
 * @param [in] relu_pre_mode Pre-ReLU parameter
 * @param [in] enable_split Enable channel split flag
 * @param [in] enable_nz2nd Enable NZ to ND conversion flag
 * @details Copies data from L0C buffer (__cc__) to Cube Buffer (__cbuf__).
 *          Supports float and int32_t data types with quantization and ReLU support.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ void *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd);

__aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ void *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd);

/*
 * @ingroup CubeDataMove
 * @brief Copy MX format data from Cube Buffer (L1) to L0A buffer
 * @param [out] dst Destination buffer address (uint64_t)
 * @param [in] src Source buffer pointer in Cube Buffer (L1) with fp8_e8m0_t format
 * @param [in] x_start_pos X starting position
 * @param [in] y_start_pos Y starting position
 * @param [in] x_step X step value
 * @param [in] y_step Y step value
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies MX (mixed precision) format data from Cube Buffer (__cbuf__) to L0A buffer.
 *          Supports fp8_e8m0_t data type for MX format operations.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_mx_sync(uint64_t dst, __cbuf__ fp8_e8m0_t *src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy MX format data from Cube Buffer (L1) to L0B buffer
 * @param [out] dst Destination buffer address (uint64_t)
 * @param [in] src Source buffer pointer in Cube Buffer (L1) with fp8_e8m0_t format
 * @param [in] x_start_pos X starting position
 * @param [in] y_start_pos Y starting position
 * @param [in] x_step X step value
 * @param [in] y_step Y step value
 * @param [in] src_stride Source stride value
 * @param [in] dst_stride Destination stride value
 * @details Copies MX (mixed precision) format data from Cube Buffer (__cbuf__) to L0B buffer.
 *          Supports fp8_e8m0_t data type for MX format operations.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_mx_sync(uint64_t dst, __cbuf__ fp8_e8m0_t *src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step, uint16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0A buffer for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in L0A
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] m_start_position M dimension starting position
 * @param [in] k_start_position K dimension starting position
 * @param [in] m_step M dimension step value
 * @param [in] k_step K dimension step value
 * @param [in] src_stride Source stride value (int16_t for signed stride support)
 * @param [in] dst_stride Destination stride value
 * @details Copies data from Cube Buffer (__cbuf__) to L0A buffer (__ca__) for 3510 architecture.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float8_e4m3_t *dst, __cbuf__ float8_e4m3_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float8_e5m2_t *dst, __cbuf__ float8_e5m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ half *dst, __cbuf__ half *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float *dst, __cbuf__ float *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ hifloat8_t *dst, __cbuf__ hifloat8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int16_t *dst, __cbuf__ int16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t *dst, __cbuf__ int32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint16_t *dst, __cbuf__ uint16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp4x2_e2m1_t *dst, __cbuf__ fp4x2_e2m1_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp4x2_e1m2_t *dst, __cbuf__ fp4x2_e1m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0A buffer with enable_transpose operation for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in L0A
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] m_start_position M dimension starting position
 * @param [in] k_start_position K dimension starting position
 * @param [in] m_step M dimension step value
 * @param [in] k_step K dimension step value
 * @param [in] src_stride Source stride value (int16_t for signed stride support)
 * @param [in] dst_stride Destination stride value
 * @details Transposes and copies data from Cube Buffer (__cbuf__) to L0A buffer (__ca__) for 3510 architecture.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ float8_e4m3_t *dst, __cbuf__ float8_e4m3_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ float8_e5m2_t *dst, __cbuf__ float8_e5m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ half *dst, __cbuf__ half *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ float *dst, __cbuf__ float *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ hifloat8_t *dst, __cbuf__ hifloat8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int16_t *dst, __cbuf__ int16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int32_t *dst, __cbuf__ int32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ uint16_t *dst, __cbuf__ uint16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ fp4x2_e2m1_t *dst, __cbuf__ fp4x2_e2m1_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ fp4x2_e1m2_t *dst, __cbuf__ fp4x2_e1m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0B buffer for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] m_start_position M dimension starting position
 * @param [in] k_start_position K dimension starting position
 * @param [in] m_step M dimension step value
 * @param [in] k_step K dimension step value
 * @param [in] src_stride Source stride value (int16_t for signed stride support)
 * @param [in] dst_stride Destination stride value
 * @details Copies data from Cube Buffer (__cbuf__) to L0B buffer (__cb__) for 3510 architecture.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float8_e4m3_t *dst, __cbuf__ float8_e4m3_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float8_e5m2_t *dst, __cbuf__ float8_e5m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ half *dst, __cbuf__ half *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float *dst, __cbuf__ float *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ hifloat8_t *dst, __cbuf__ hifloat8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int16_t *dst, __cbuf__ int16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint16_t *dst, __cbuf__ uint16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp4x2_e2m1_t *dst, __cbuf__ fp4x2_e2m1_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp4x2_e1m2_t *dst, __cbuf__ fp4x2_e1m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0B buffer with enable_transpose operation for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] m_start_position M dimension starting position
 * @param [in] k_start_position K dimension starting position
 * @param [in] m_step M dimension step value
 * @param [in] k_step K dimension step value
 * @param [in] src_stride Source stride value (int16_t for signed stride support)
 * @param [in] dst_stride Destination stride value
 * @details Transposes and copies data from Cube Buffer (__cbuf__) to L0B buffer (__cb__) for 3510 architecture.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t, int4b_t, fp4x2_e2m1_t, fp4x2_e1m2_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ float8_e4m3_t *dst, __cbuf__ float8_e4m3_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ float8_e5m2_t *dst, __cbuf__ float8_e5m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ half *dst, __cbuf__ half *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ float *dst, __cbuf__ float *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ hifloat8_t *dst, __cbuf__ hifloat8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int16_t *dst, __cbuf__ int16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ uint16_t *dst, __cbuf__ uint16_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ fp4x2_e2m1_t *dst, __cbuf__ fp4x2_e2m1_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

__aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ fp4x2_e1m2_t *dst, __cbuf__ fp4x2_e1m2_t *src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0B buffer with enable_transpose operation for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] index_id Index identifier
 * @param [in] repeat Number of repeat operations
 * @param [in] src_stride Source stride value
 * @param [in] dst_gap Destination stride value
 * @param [in] dst_frac_gap Destination fractional stride
 * @param [in] src_frac_gap Source fractional stride
 * @details Transposes and copies data from Cube Buffer (__cbuf__) to L0B buffer (__cb__) for 3510 architecture.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int32_t, int8_t, uint32_t, uint8_t, float4_e1m2x2_t, float4_e2m1x2_t, int4b_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ float8_e4m3_t *dst, __cbuf__ float8_e4m3_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ float8_e5m2_t *dst, __cbuf__ float8_e5m2_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ half *dst, __cbuf__ half *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ float *dst, __cbuf__ float *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ hifloat8_t *dst, __cbuf__ hifloat8_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ float4_e1m2x2_t *dst, __cbuf__ float4_e1m2x2_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ float4_e2m1x2_t *dst, __cbuf__ float4_e2m1x2_t *src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

__aicore__ inline void asc_copy_l12l0b_trans_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to Global Memory for various data types
 * @param [out] dst Destination buffer pointer in Global Memory
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] burst_count Number of burst operations
 * @param [in] burst_len Length of each burst
 * @param [in] src_gap Source stride value
 * @param [in] dst_gap Destination stride value
 * @details Copies data from Cube Buffer (__cbuf__) to Global Memory (__gm__).
 *          Supports void pointer interface for various data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12gm_sync(__gm__ void *dst, __cbuf__ void *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0C buffer for various data types
 * @param [out] dst Destination buffer pointer in L0C
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] burst_count Number of burst operations
 * @param [in] burst_len Length of each burst
 * @param [in] src_stride Source stride value
 * @param [in] dst_gap Destination gap value
 * @details Copies data from Cube Buffer (__cbuf__) to L0C buffer (__cc__).
 *          Supports bfloat16_t, half, float, int32_t, uint32_t data types.
 *          Also supports half and bfloat16_t destination with float source for type conversion.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0c_sync(__cc__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(__cc__ half *dst, __cbuf__ half *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(__cc__ half *dst, __cbuf__ float *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(__cc__ bfloat16_t *dst, __cbuf__ float *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(__cc__ float *dst, __cbuf__ float *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(__cc__ int32_t *dst, __cbuf__ int32_t *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12l0c_sync(__cc__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride, uint16_t dst_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0A buffer for convolution operations (3510 architecture)
 * @param [out] dst Destination buffer pointer in L0A
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] k_extension K dimension extension value
 * @param [in] m_extension M dimension extension value
 * @param [in] k_start_pt K dimension starting point
 * @param [in] m_start_pt M dimension starting point
 * @param [in] stride_w Width stride value
 * @param [in] stride_h Height stride value
 * @param [in] filter_w Filter width value
 * @param [in] filter_h Filter height value
 * @param [in] dilation_filter_w Filter width dilation value
 * @param [in] dilation_filter_h Filter height dilation value
 * @param [in] enable_filter_size_w Filter size width flag
 * @param [in] enable_filter_size_h Filter size height flag
 * @param [in] enable_transpose enable_transpose operation flag
 * @param [in] fmatrix_ctrl Feature matrix control flag
 * @param [in] channel_size Channel size value
 * @details Copies data from Cube Buffer (__cbuf__) to L0A buffer (__ca__) for convolution operations.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int32_t, int8_t, uint32_t, uint8_t, int16_t, uint16_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float8_e4m3_t *dst, __cbuf__ float8_e4m3_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float8_e5m2_t *dst, __cbuf__ float8_e5m2_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ half *dst, __cbuf__ half *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ float *dst, __cbuf__ float *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ hifloat8_t *dst, __cbuf__ hifloat8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t *dst, __cbuf__ int32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t *dst, __cbuf__ int8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ int16_t *dst, __cbuf__ int16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint16_t *dst, __cbuf__ uint16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to L0B buffer for convolution operations (3510 architecture)
 * @param [out] dst Destination buffer pointer in L0B
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] k_extension K dimension extension value
 * @param [in] m_extension M dimension extension value
 * @param [in] k_start_pt K dimension starting point
 * @param [in] m_start_pt M dimension starting point
 * @param [in] stride_w Width stride value
 * @param [in] stride_h Height stride value
 * @param [in] filter_w Filter width value
 * @param [in] filter_h Filter height value
 * @param [in] dilation_filter_w Filter width dilation value
 * @param [in] dilation_filter_h Filter height dilation value
 * @param [in] enable_filter_size_w Filter size width flag
 * @param [in] enable_filter_size_h Filter size height flag
 * @param [in] enable_transpose enable_transpose operation flag
 * @param [in] fmatrix_ctrl Feature matrix control flag
 * @param [in] channel_size Channel size value
 * @details Copies data from Cube Buffer (__cbuf__) to L0B buffer (__cb__) for convolution operations.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int32_t, int8_t, uint32_t, uint8_t, int16_t, uint16_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t *dst, __cbuf__ bfloat16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float8_e4m3_t *dst, __cbuf__ float8_e4m3_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float8_e5m2_t *dst, __cbuf__ float8_e5m2_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ half *dst, __cbuf__ half *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float *dst, __cbuf__ float *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ hifloat8_t *dst, __cbuf__ hifloat8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t *dst, __cbuf__ int32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t *dst, __cbuf__ int8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t *dst, __cbuf__ uint32_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t *dst, __cbuf__ uint8_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int16_t *dst, __cbuf__ int16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint16_t *dst, __cbuf__ uint16_t *src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool enable_filter_size_w, bool enable_filter_size_h, bool enable_transpose, bool fmatrix_ctrl, uint16_t channel_size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with alignment and padding support for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] burst_count Number of burst operations
 * @param [in] burst_len Length of each burst
 * @param [in] left_padding_count Left padding count
 * @param [in] right_padding_count Right padding count
 * @param [in] enable_data_select Data selection bit
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] burst_src_stride Burst source stride (uint64_t)
 * @param [in] burst_dst_stride Burst destination stride
 * @details Copies aligned data from Global Memory (__gm__) to Cube Buffer (__cbuf__) with padding.
 *          Supports bfloat16_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ half *dst, __gm__ half *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ float *dst, __gm__ float *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ hifloat8_t *dst, __gm__ hifloat8_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int16_t *dst, __gm__ int16_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint16_t *dst, __gm__ uint16_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, uint8_t l2_cache_ctrl, uint64_t burst_src_stride, uint32_t burst_dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with DN to NZ format conversion for various data types
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1) with NZ format
 * @param [in] src Source buffer pointer in Global Memory with DN format
 * @param [in] loop1_src_stride Loop 1 source stride (uint64_t)
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] n_value N dimension value
 * @param [in] d_value D dimension value (uint32_t)
 * @param [in] loop4_src_stride Loop 4 source stride (uint64_t)
 * @param [in] enable_smallc0 Small C0 enable flag
 * @details Converts DN format data from Global Memory (__gm__) to NZ format in Cube Buffer (__cbuf__).
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ float8_e4m3_t *dst, __gm__ float8_e4m3_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ float8_e5m2_t *dst, __gm__ float8_e5m2_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ half *dst, __gm__ half *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ float *dst, __gm__ float *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ hifloat8_t *dst, __gm__ hifloat8_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ int16_t *dst, __gm__ int16_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ uint16_t *dst, __gm__ uint16_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with ND to NZ format conversion for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1) with NZ format
 * @param [in] src Source buffer pointer in Global Memory with ND format
 * @param [in] loop1_src_stride Loop 1 source stride (uint64_t)
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] n_value N dimension value
 * @param [in] d_value D dimension value (uint32_t)
 * @param [in] loop4_src_stride Loop 4 source stride (uint64_t)
 * @param [in] enable_smallc0 Small C0 enable flag
 * @details Converts ND format data from Global Memory (__gm__) to NZ format in Cube Buffer (__cbuf__) for 3510 architecture.
 *          Supports bfloat16_t, float8_e4m3_t, float8_e5m2_t, half, float, hifloat8_t, int16_t, int32_t, int8_t, uint16_t, uint32_t, uint8_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ float8_e4m3_t *dst, __gm__ float8_e4m3_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ float8_e5m2_t *dst, __gm__ float8_e5m2_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ half *dst, __gm__ half *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ float *dst, __gm__ float *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ hifloat8_t *dst, __gm__ hifloat8_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ int16_t *dst, __gm__ int16_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ uint16_t *dst, __gm__ uint16_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint64_t loop1_src_stride, uint8_t l2_cache_ctrl, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool enable_smallc0);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with padding function for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in Global Memory
 * @param [in] burst_count Number of burst operations
 * @param [in] burst_len Length of each burst (uint32_t)
 * @param [in] pad_func_mode Padding function mode
 * @param [in] src_stride Source stride value (uint64_t)
 * @param [in] dst_stride Destination stride value (uint32_t)
 * @details Copies data from Global Memory (__gm__) to Cube Buffer (__cbuf__) with padding function for 3510 architecture.
 *          Supports void pointer interface for various data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t burst_count, uint32_t burst_len, uint8_t pad_func_mode, uint64_t src_stride, uint32_t dst_stride);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to BT buffer for various data types (3510 architecture)
 * @param [out] dst Destination buffer address (uint64_t)
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] size Data size in bytes to copy
 * @details Copies data from Cube Buffer (__cbuf__) to BT buffer for 3510 architecture.
 *          Supports bfloat16_t, half, float, int32_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t *src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half *src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float *src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t *src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to Feature Buffer (L1) version 2 for various data types
 * @param [out] dst Destination buffer pointer in Feature Buffer (L1)
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] size Data size in bytes to copy
 * @details Copies data from Cube Buffer (__cbuf__) to Feature Buffer (__fbuf__) in L1 memory (version 2).
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12fb_v2_sync(__fbuf__ void *dst, __cbuf__ void *src, uint32_t size);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Cube Buffer (L1) to Unified Buffer for various data types
 * @param [out] dst Destination buffer pointer in Unified Buffer
 * @param [in] src Source buffer pointer in Cube Buffer (L1)
 * @param [in] sub_blockid Sub-block ID flag
 * @param [in] burst_count Number of burst operations
 * @param [in] burst_len Length of each burst
 * @param [in] src_gap Source gap value
 * @param [in] dst_gap Destination gap value
 * @details Copies data from Cube Buffer (__cbuf__) to Unified Buffer (__ubuf__).
 *          Supports void pointer interface for various data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l12ub_sync(__ubuf__ void *dst, __cbuf__ void *src, bool sub_blockid, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to Cube Buffer (L1) with full quantization support for various data types (3510 architecture)
 * @param [out] dst Destination buffer pointer in Cube Buffer (L1)
 * @param [in] src Source buffer pointer in L0C
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Destination stride value (uint32_t)
 * @param [in] src_stride Source stride value
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] clip_relu_pre_mode Pre-clip ReLU parameter
 * @param [in] unit_flag Unit flag mode setting
 * @param [in] quant_pre_mode Pre-quantization parameter (uint64_t)
 * @param [in] relu_pre_mode Pre-ReLU parameter
 * @param [in] enable_channel_split Channel split enable flag
 * @param [in] enable_nz2nd NZ to ND conversion enable flag
 * @param [in] quant_post_mode Post-quantization parameter (uint64_t)
 * @param [in] relu_post_mode Post-ReLU parameter
 * @param [in] clip_relu_post_mode Post-clip ReLU flag
 * @param [in] eltwise_op Element-wise operation mode
 * @param [in] enable_antiquant Element-wise anti-quantization enable flag
 * @param [in] enable_c0_pad C0 padding enable flag
 * @param [in] enable_broadcast Broadcast enable flag
 * @param [in] enable_nz2dn NZ to DN conversion enable flag
 * @details Copies data from L0C buffer (__cc__) to Cube Buffer (__cbuf__) with full quantization and ReLU support for 3510 architecture.
 *          Supports float and int32_t data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ void *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ void *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to Global Memory with full quantization support for various data types
 * @param [out] dst Destination buffer pointer in Global Memory
 * @param [in] src Source buffer pointer in L0C
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Loop destination stride value (uint32_t)
 * @param [in] src_stride Loop source stride value
 * @param [in] l2_cache_ctrl L2 cache control value
 * @param [in] clip_relu_pre_mode Pre-clip ReLU parameter
 * @param [in] unit_flag Unit flag control setting
 * @param [in] quant_pre_mode Pre-quantization parameter (uint64_t)
 * @param [in] relu_pre_mode Pre-ReLU parameter
 * @param [in] enable_split Split enable flag
 * @param [in] enable_nz2nd NZ to ND conversion enable flag
 * @param [in] quant_post_mode Post-quantization parameter (uint64_t)
 * @param [in] relu_post_mode Post-ReLU parameter
 * @param [in] clip_relu_post_mode Post-clip ReLU flag
 * @param [in] eltwise_op Element-wise operation mode
 * @param [in] enable_antiquant Element-wise anti-quantization enable flag
 * @param [in] enable_c0_pad C0 padding enable flag
 * @param [in] enable_broadcast Broadcast enable flag
 * @param [in] enable_nz2dn NZ to DN conversion enable flag
 * @details Copies data from L0C buffer (__cc__) to Global Memory (__gm__) with full quantization and ReLU support.
 *          Supports bfloat16_t, half, float8_e4m3_t, float8_e5m2_t, hifloat8_t, int8_t, uint8_t, float, int32_t, void data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ bfloat16_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ half *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ float8_e4m3_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ float8_e5m2_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ hifloat8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ int8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ uint8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ float *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ bfloat16_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ half *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ float8_e4m3_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ float8_e5m2_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ hifloat8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ int8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ uint8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ int32_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ void *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2gm_sync(__gm__ void *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t l2_cache_ctrl, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from L0C buffer to Unified Buffer with full quantization support for various data types
 * @param [out] dst Destination buffer pointer in Unified Buffer
 * @param [in] src Source buffer pointer in L0C
 * @param [in] n_size N dimension size
 * @param [in] m_size M dimension size
 * @param [in] dst_stride Loop destination stride value (uint32_t)
 * @param [in] src_stride Loop source stride value
 * @param [in] dual_dst_ctl Dual destination control value
 * @param [in] sub_blockid Sub-block ID flag
 * @param [in] clip_relu_pre_mode Pre-clip ReLU parameter
 * @param [in] unit_flag Unit flag control setting
 * @param [in] quant_pre_mode Pre-quantization parameter (uint64_t)
 * @param [in] relu_pre_mode Pre-ReLU parameter
 * @param [in] enable_split Split enable flag
 * @param [in] enable_nz2nd NZ to ND conversion enable flag
 * @param [in] quant_post_mode Post-quantization parameter (uint64_t)
 * @param [in] relu_post_mode Post-ReLU parameter
 * @param [in] clip_relu_post_mode Post-clip ReLU flag
 * @param [in] eltwise_op Element-wise operation mode
 * @param [in] enable_antiquant Element-wise anti-quantization enable flag
 * @param [in] enable_c0_pad C0 padding enable flag
 * @param [in] enable_broadcast Broadcast enable flag
 * @param [in] enable_nz2dn NZ to DN conversion enable flag
 * @details Copies data from L0C buffer (__cc__) to Unified Buffer (__ubuf__) with full quantization and ReLU support.
 *          Supports bfloat16_t, half, float8_e4m3_t, float8_e5m2_t, hifloat8_t, int8_t, uint8_t, float, int32_t, void data types.
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ bfloat16_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ half *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ float8_e4m3_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ float8_e5m2_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ hifloat8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ int8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ uint8_t *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ float *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ bfloat16_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ half *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ float8_e4m3_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ float8_e5m2_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ hifloat8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ int8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ uint8_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ int32_t *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ void *dst, __cc__ float *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ void *dst, __cc__ int32_t *src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t dual_dst_ctl, bool sub_blockid, uint8_t clip_relu_pre_mode, uint8_t unit_flag, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_split, bool enable_nz2nd, uint64_t quant_post_mode, uint8_t relu_post_mode, bool enable_clip_relu_post_mode, uint8_t eltwise_op, bool enable_antiquant, bool enable_c0_pad, bool enable_broadcast, bool enable_nz2dn);

/*
 * @ingroup CubeDataMove
 * @brief Copy data from Global Memory to Cube Buffer (L1) with decomposition mode for various data types (3510 architecture)
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
 *          This is a software emulation interface with automatic synchronization.
 */
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ bfloat16_t *dst, __gm__ bfloat16_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float *dst, __gm__ float *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float8_e4m3_t *dst, __gm__ float8_e4m3_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float8_e5m2_t *dst, __gm__ float8_e5m2_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ half *dst, __gm__ half *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ hifloat8_t *dst, __gm__ hifloat8_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int16_t *dst, __gm__ int16_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int32_t *dst, __gm__ int32_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int8_t *dst, __gm__ int8_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint16_t *dst, __gm__ uint16_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint32_t *dst, __gm__ uint32_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint8_t *dst, __gm__ uint8_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float4_e1m2x2_t *dst, __gm__ float4_e1m2x2_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float4_e2m1x2_t *dst, __gm__ float4_e2m1x2_t *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);

__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void *dst, __gm__ void *src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctrl);
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
