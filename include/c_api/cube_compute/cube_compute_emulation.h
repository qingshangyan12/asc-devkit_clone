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
 * @file cube_compute_emulation.h
 * @brief This file provides software simulation interfaces for cube compute operations.
 * @details These interfaces are designed to improve usability. Users do not need to add
 *          synchronization operations when using these interfaces. All interfaces in this
 *          file have built-in synchronization support.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif     

#ifndef INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_EMULATION_H
#define INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_EMULATION_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "instr_impl/npu_arch_2201/cube_compute_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "instr_impl/npu_arch_3510/cube_compute_impl.h"

#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with bfloat16 inputs and float output (synchronized)
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
 *          This is a synchronized version that includes built-in synchronization.
 */
__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b,
    uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ half* a, __cb__ half* b, 
    uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float* a, __cb__ float* b, 
    uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, 
    uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with bfloat16 inputs and float output with offset (synchronized)
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
 * @param [in] enable_weight_offset Whether weight offset is enabled
 * @param [in] enable_cube_init Whether to initialize cube with bias buffer
 * @param [in] enable_cube_zero_init Whether to initialize cube to zero
 * @details Performs C = A * B matrix multiplication with bfloat16 inputs and float output,
 *          supporting offset parameters. This is a synchronized version.
 */
__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b,
    uint16_t m,  uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool enable_k_direction_align, bool enable_weight_offset, bool enable_cube_init, bool enable_cube_zero_init);


__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ half* a, __cb__ half* b, 
    uint16_t m, uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool enable_k_direction_align, bool enable_weight_offset, bool enable_cube_init, bool enable_cube_zero_init);


__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float* a, __cb__ float* b,
    uint16_t m, uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag,
    bool enable_k_direction_align, bool enable_weight_offset,
    bool enable_cube_init, bool enable_cube_zero_init);


__aicore__ inline void asc_mmad_sync(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b,
    uint16_t m, uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, 
    bool enable_k_direction_align, bool enable_weight_offset, bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with int4 inputs and int32 output (synchronized)
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
 *          This is a synchronized version that includes built-in synchronization.
 */
__aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c, __ca__ int4b_t* a, __cb__ int4b_t* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t unit_flag, bool enable_k_direction_align, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_s4_sync(__cc__ int32_t* c, __ca__ int4b_t* a, __cb__ int4b_t* b, uint16_t m,
     uint16_t n, uint16_t k, uint8_t feat_offset, uint8_t smask_offset, uint8_t unit_flag, bool enable_k_direction_align, bool enable_weight_offset,
     bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Sparse matrix multiply-add operation with int8 inputs and int32 output (synchronized)
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
 *          This is a synchronized version that includes built-in synchronization.
 */
__aicore__ inline void asc_mmad_sparse_sync(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m,
                                             uint16_t k, uint16_t n, uint8_t unit_flag, bool cmatrix_source,
                                             bool cmatrix_init_val);

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with MX format inputs (e1m2 * e1m2) and float output (synchronized)
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
 *          This is a synchronized version that includes built-in synchronization.
 */
__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
     __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e1m2_t* a,
     __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
     __cb__ fp4x2_e1m2_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp4x2_e2m1_t* a,
     __cb__ fp4x2_e2m1_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
     __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e4m3fn_t* a,
     __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e5m2_t* a,
     __cb__ fp8_e4m3fn_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_mx_sync(__cc__ float* c, __ca__ fp8_e5m2_t* a,
     __cb__ fp8_e5m2_t* b, uint16_t m, uint16_t n, uint16_t k,
     uint8_t unit_flag, bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

/*
 * @ingroup CubeCompute
 * @brief Matrix multiply-add operation with bfloat16 inputs and float output (synchronized)
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
 *          This is a synchronized version that includes built-in synchronization.
 */
__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ bfloat16_t* a, __cb__ bfloat16_t* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float8_e4m3_t* a,
                                      __cb__ float8_e4m3_t* b, uint16_t m, uint16_t n,
                                      uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                      bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float8_e4m3_t* a,
                                      __cb__ float8_e5m2_t* b, uint16_t m, uint16_t n,
                                      uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                      bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float8_e5m2_t* a,
                                      __cb__ float8_e4m3_t* b, uint16_t m, uint16_t n,
                                      uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                      bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float8_e5m2_t* a,
                                      __cb__ float8_e5m2_t* b, uint16_t m, uint16_t n,
                                      uint16_t k, uint8_t unit_flag, bool disable_gemv, bool enable_cube_init,
                                      bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ half* a, __cb__ half* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ float* c, __ca__ float* a, __cb__ float* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

__aicore__ inline void asc_mmad_sync(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b,
                                     uint16_t m, uint16_t n, uint16_t k, uint8_t unit_flag,
                                     bool disable_gemv, bool enable_cube_init, bool enable_cube_zero_init);

#endif

#endif // INCLUDE_C_API_CUBE_COMPUTE_CUBE_COMPUTE_EMULATION_H

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif
