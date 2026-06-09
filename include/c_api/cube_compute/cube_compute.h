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
 * @file cube_compute.h
 * @brief This file provides hardware implementation interfaces for cube compute operations.
 * @details These interfaces are low-level hardware interfaces. Users need to manually add
 *          synchronization operations when using these interfaces. For ease of use, prefer
 *          the synchronized versions provided in cube_compute_emulation.h.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif     

#ifndef INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_H
#define INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "instr_impl/npu_arch_2201/cube_compute_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "instr_impl/npu_arch_3510/cube_compute_impl.h"

#endif

/*
 * @ingroup CubeCompute
 * @brief Set L0C to GM configuration
 * @param [in] relu_pre ReLU configuration parameter
 * @param [in] quant_pre Quantization configuration parameter
 * @param [in] enable_unit_flag Whether to enable unit flag
 * @details Configure L0C to GM transfer parameters for subsequent matrix operations
 */
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with bfloat16 inputs and float output
 * @param [out] c Output matrix pointer, stored in L0C buffer
 * @param [in] a Input matrix A pointer, stored in L0A buffer
 * @param [in] b Input matrix B pointer, stored in L0B buffer
 * @param [in] m Rows of matrix A
 * @param [in] n Columns of matrix B
 * @param [in] k Columns of matrix A / Rows of matrix B
 * @param [in] unit_flag Unit flag for operation control
 * @param [in] enable_k_direction_align Whether to enable K direction alignment
 * @param [in] enable_cube_init Whether to initialize cube
 * @param [in] enable_cube_zero_init Whether to initialize cube to zero
 * @details Performs C = A * B matrix multiplication with bfloat16 inputs and float output.
 *          This is a hardware-level interface that requires manual synchronization.
 */
__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b, uint16_t m,
    uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ half* a, __cb__ half* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float* a, __cb__ float* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with bfloat16 inputs and float output with offset
 * @param [out] c Output matrix pointer, stored in L0C buffer
 * @param [in] a Input matrix A pointer, stored in L0A buffer
 * @param [in] b Input matrix B pointer, stored in L0B buffer
 * @param [in] m Rows of matrix A
 * @param [in] n Columns of matrix B
 * @param [in] k Columns of matrix A / Rows of matrix B
 * @param [in] feat_offset Feature offset for operation
 * @param [in] smask_offset Smask offset for operation
 * @param [in] unit_flag Unit flag for operation control
 * @param [in] enable_k_direction_align Whether to enable K direction alignment
 * @param [in] is_weight_offset Whether weight offset is enabled
 * @param [in] enable_cube_init Whether to initialize cube
 * @param [in] enable_cube_zero_init Whether to initialize cube to zero
 * @details Performs C = A * B matrix multiplication with bfloat16 inputs and float output,
 *          supporting offset parameters. This is a hardware-level interface.
 */
__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b,
    uint16_t m,  uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool enable_k_direction_align, bool is_weight_offset, bool enable_cube_init, bool enable_cube_zero_init);


__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ half* a, __cb__ half* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool enable_k_direction_align,
    bool is_weight_offset, bool enable_cube_init, bool enable_cube_zero_init);


__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float* a, __cb__ float* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool enable_k_direction_align,
    bool is_weight_offset, bool enable_cube_init, bool enable_cube_zero_init);


__aicore__ inline void asc_mmad(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool enable_k_direction_align,
    bool is_weight_offset, bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with int4 inputs and int32 output
 * @param [out] c Output matrix pointer, stored in L0C buffer
 * @param [in] a Input matrix A pointer, stored in L0A buffer
 * @param [in] b Input matrix B pointer, stored in L0B buffer
 * @param [in] m Rows of matrix A
 * @param [in] n Columns of matrix B
 * @param [in] k Columns of matrix A / Rows of matrix B
 * @param [in] unit_flag Unit flag for operation control
 * @param [in] enable_k_direction_align Whether to enable K direction alignment
 * @param [in] enable_cube_init Whether to initialize cube
 * @param [in] enable_cube_zero_init Whether to initialize cube to zero
 * @details Performs C = A * B matrix multiplication with int4 inputs and int32 output.
 *          This is a hardware-level interface that requires manual synchronization.
 */
__aicore__ inline void asc_mmad_s4(__cc__ int32_t* c, __ca__ int4b_t* a, __cb__ int4b_t* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_s4(__cc__ int32_t* c, __ca__ int4b_t* a, __cb__ int4b_t* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool enable_k_direction_align, bool is_weight_offset,
    bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Sparse matrix multiply-add operation with int8 inputs and int32 output
 * @param [out] c Output matrix pointer, stored in L0C buffer
 * @param [in] a Input matrix A pointer, stored in L0A buffer
 * @param [in] b Input matrix B pointer, stored in L0B buffer
 * @param [in] m Rows of matrix A
 * @param [in] k Columns of matrix A / Rows of matrix B
 * @param [in] n Columns of matrix B
 * @param [in] unit_flag Unit flag for operation control
 * @param [in] cmatrix_source Source of C matrix
 * @param [in] cmatrix_init_val Initial value of C matrix
 * @details Performs sparse C = A * B matrix multiplication with int8 inputs and int32 output.
 *          This is a hardware-level interface that requires manual synchronization.
 */
__aicore__ inline void asc_mmad_sparse(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k,
                                       uint16_t n, uint8_t unit_flag, bool cmatrix_source, bool cmatrix_init_val);

/*
 * @ingroup CubeCompute
 * @brief Set MMAD computation direction to M direction
 * @details Configure the matrix multiplication direction to optimize M direction computation
 */
__aicore__ inline void asc_set_mmad_direction_m();

/*
 * @ingroup CubeCompute
 * @brief Set MMAD computation direction to N direction
 * @details Configure the matrix multiplication direction to optimize N direction computation
 */
__aicore__ inline void asc_set_mmad_direction_n();

/*
 * @ingroup CubeCompute
 * @brief Enable HF32 transformation mode
 * @param [in] mode HF32 transformation mode value
 * @details Configure HF32 transformation mode for specific precision requirements
 */
__aicore__ inline void asc_enable_hf32_trans(uint32_t mode);

/*
 * @ingroup CubeCompute
 * @brief Enable HF32 mode
 * @details Enable HF32 precision mode for matrix operations
 */
__aicore__ inline void asc_enable_hf32();

/*
 * @ingroup CubeCompute
 * @brief Set FP32 mode
 * @details Configure FP32 precision mode for matrix operations
 */
__aicore__ inline void asc_set_fp32_mode();

/*
 * @ingroup CubeCompute
 * @brief Get L0C to GM ReLU configuration
 * @return ReLU configuration value
 * @details Retrieve the current ReLU configuration for L0C to GM transfer
 */
__aicore__ inline uint64_t asc_get_l0c2gm_relu();

/*
 * @ingroup CubeCompute
 * @brief Get L0C to GM unit flag configuration
 * @return Unit flag configuration value
 * @details Retrieve the current unit flag configuration for L0C to GM transfer
 */
__aicore__ inline uint64_t asc_get_l0c2gm_unitflag();

/*
 * @ingroup CubeCompute
 * @brief Get L0C to GM pre-quantization configuration
 * @return Pre-quantization configuration value
 * @details Retrieve the current pre-quantization configuration for L0C to GM transfer
 */
__aicore__ inline uint64_t asc_get_l0c2gm_prequant();

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
/*
 * @ingroup CubeCompute
 * @brief Set L0C to GM NZ to ND format conversion configuration
 * @param [in] nd_num Number of ND blocks
 * @param [in] src_nd_stride Source ND stride
 * @param [in] dst_nd_stride Destination ND stride
 * @details Configure the NZ to ND format conversion parameters for L0C to GM transfer
 */
__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with MX format inputs (e1m2 * e1m2) and float output
 * @param [out] c Output matrix pointer, stored in L0C buffer
 * @param [in] a Input matrix A pointer with fp4x2_e1m2_t format, stored in L0A buffer
 * @param [in] b Input matrix B pointer with fp4x2_e1m2_t format, stored in L0B buffer
 * @param [in] m Rows of matrix A
 * @param [in] n Columns of matrix B
 * @param [in] k Columns of matrix A / Rows of matrix B
 * @param [in] unit_flag Unit flag for operation control
 * @param [in] disable_gemv Whether to disable GEMV optimization
 * @param [in] enable_cube_init Whether to initialize cube
 * @param [in] enable_cube_zero_init Whether to initialize cube to zero
 * @details Performs C = A * B matrix multiplication with MX format (e1m2) inputs and float output.
 *          This is a hardware-level interface that requires manual synchronization.
 */
__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
    __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
    __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
    __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
    __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with FP8 format inputs (e4m3fn * e4m3fn) and float output
 * @param [out] c Output matrix pointer, stored in L0C buffer
 * @param [in] a Input matrix A pointer with fp8_e4m3fn_t format, stored in L0A buffer
 * @param [in] b Input matrix B pointer with fp8_e4m3fn_t format, stored in L0B buffer
 * @param [in] m Rows of matrix A
 * @param [in] n Columns of matrix B
 * @param [in] k Columns of matrix A / Rows of matrix B
 * @param [in] unit_flag Unit flag for operation control
 * @param [in] disable_gemv Whether to disable GEMV optimization
 * @param [in] enable_cube_init Whether to initialize cube
 * @param [in] enable_cube_zero_init Whether to initialize cube to zero
 * @details Performs C = A * B matrix multiplication with FP8 format (e4m3fn) inputs and float output.
 *          This is a hardware-level interface that requires manual synchronization.
 */
__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
    __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
    __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e5m2_t* a,
    __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx(__cc__ float* c, __ca__ fp8_e5m2_t* a,
    __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
    uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with bfloat16 inputs and float output
 * @param [out] c Output matrix pointer, stored in L0C buffer
 * @param [in] a Input matrix A pointer, stored in L0A buffer
 * @param [in] b Input matrix B pointer, stored in L0B buffer
 * @param [in] m Rows of matrix A
 * @param [in] n Columns of matrix B
 * @param [in] k Columns of matrix A / Rows of matrix B
 * @param [in] unit_flag Unit flag for operation control
 * @param [in] disable_gemv Whether to disable GEMV optimization
 * @param [in] enable_cube_init Whether to initialize cube
 * @param [in] enable_cube_zero_init Whether to initialize cube to zero
 * @details Performs C = A * B matrix multiplication with bfloat16 inputs and float output.
 *          This is a hardware-level interface that requires manual synchronization.
 */
__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float8_e4m3_t* a, __cb__ float8_e4m3_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float8_e4m3_t* a, __cb__ float8_e5m2_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float8_e5m2_t* a, __cb__ float8_e4m3_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float8_e5m2_t* a, __cb__ float8_e5m2_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ half* a, __cb__ half* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ float* c, __ca__ float* a, __cb__ float* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b,
                                uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

#endif

#endif // INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_H

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif
