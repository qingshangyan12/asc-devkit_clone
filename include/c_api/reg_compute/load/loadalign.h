/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "reg_compute/load/loadalign.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_LOAD_LOADALIGN_H
#define INCLUDE_C_API_REG_COMPUTE_LOAD_LOADALIGN_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/loadalign_intf_impl.h"
#endif

// ========== return-value load APIs ==========
// ========== return vector data register APIs ==========
// ========== asc_loadalign ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUu8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUu16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUi32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Returns a vector loaded from a 32-byte-aligned UB address without changing its data format.
 *
 * @override_id asc_loadalign__PUf32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign(__ubuf__ float* src);

// ========== asc_loadalign_brc_datablock ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_datablock(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUi8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_brc_datablock(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUu8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_brc_datablock(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_datablock(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_datablock(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_datablock(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_datablock(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUi16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_brc_datablock(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUu16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_datablock(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_brc_datablock(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_datablock(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUi32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_brc_datablock(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_datablock(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float return-value overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_datablock__PUf32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_brc_datablock(__ubuf__ float* src);

// ========== asc_loadalign_brc_elem ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_elem(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUi8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_brc_elem(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUu8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_brc_elem(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_elem(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_elem(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_elem(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_elem(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUi16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUu16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_brc_elem(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUi32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float return-value overload: reads a single element from the dtype-aligned UB address src and returns a vector register broadcasting it to all lanes.
 *
 * @override_id asc_loadalign_brc_elem__PUf32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_brc_elem(__ubuf__ float* src);

// ========== asc_loadalign_brc_elem2datablock ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int16 elements from 16-byte-aligned UB src and returns a vector register where each element fills one 32-byte DataBlock.
 *
 * @override_id asc_loadalign_brc_elem2datablock__PUi16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem2datablock(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint16 elements from 16-byte-aligned UB src and returns a vector register where each element fills one 32-byte DataBlock.
 *
 * @override_id asc_loadalign_brc_elem2datablock__PUu16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem2datablock(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 half elements from 16-byte-aligned UB src and returns a vector register where each element fills one 32-byte DataBlock.
 *
 * @override_id asc_loadalign_brc_elem2datablock__PUf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_brc_elem2datablock(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 bfloat16 elements from 16-byte-aligned UB src and returns a vector register where each element fills one 32-byte DataBlock.
 *
 * @override_id asc_loadalign_brc_elem2datablock__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem2datablock(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int32 elements from 32-byte-aligned UB src and returns a vector register where each element fills one 32-byte DataBlock.
 *
 * @override_id asc_loadalign_brc_elem2datablock__PUi32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem2datablock(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint32 elements from 32-byte-aligned UB src and returns a vector register where each element fills one 32-byte DataBlock.
 *
 * @override_id asc_loadalign_brc_elem2datablock__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem2datablock(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 float elements from 32-byte-aligned UB src and returns a vector register where each element fills one 32-byte DataBlock.
 *
 * @override_id asc_loadalign_brc_elem2datablock__PUf32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_brc_elem2datablock(__ubuf__ float* src);

// ========== asc_loadalign_downsample ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_int4x2_t.
 *
 * @override_id asc_loadalign_downsample__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_downsample(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_int8_t.
 *
 * @override_id asc_loadalign_downsample__PUi8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_downsample(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_uint8_t.
 *
 * @override_id asc_loadalign_downsample__PUu8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_downsample(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_fp4x2_e2m1_t.
 *
 * @override_id asc_loadalign_downsample__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_downsample(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_fp4x2_e1m2_t.
 *
 * @override_id asc_loadalign_downsample__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_downsample(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_hifloat8_t.
 *
 * @override_id asc_loadalign_downsample__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_downsample(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_fp8_e8m0_t.
 *
 * @override_id asc_loadalign_downsample__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_downsample(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_fp8_e5m2_t.
 *
 * @override_id asc_loadalign_downsample__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_downsample(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_fp8_e4m3fn_t.
 *
 * @override_id asc_loadalign_downsample__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_downsample(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_int16_t.
 *
 * @override_id asc_loadalign_downsample__PUi16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_downsample(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_uint16_t.
 *
 * @override_id asc_loadalign_downsample__PUu16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_downsample(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_half.
 *
 * @override_id asc_loadalign_downsample__PUf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_downsample(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads 2*VL (512 bytes) of contiguous data from a 32-byte-aligned UB src, keeps even-index elements, and returns the VL-length downsampled vector_bfloat16_t.
 *
 * @override_id asc_loadalign_downsample__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_downsample(__ubuf__ bfloat16_t* src);

// ========== asc_loadalign_unpack ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous int4b_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_int4x2_t.
 *
 * @override_id asc_loadalign_unpack__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous int8_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_int8_t.
 *
 * @override_id asc_loadalign_unpack__PUi8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_unpack(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous uint8_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_uint8_t.
 *
 * @override_id asc_loadalign_unpack__PUu8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_unpack(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous fp4x2_e2m1_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_fp4x2_e2m1_t.
 *
 * @override_id asc_loadalign_unpack__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous fp4x2_e1m2_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_fp4x2_e1m2_t.
 *
 * @override_id asc_loadalign_unpack__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous hifloat8_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_hifloat8_t.
 *
 * @override_id asc_loadalign_unpack__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous fp8_e8m0_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_fp8_e8m0_t.
 *
 * @override_id asc_loadalign_unpack__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous fp8_e5m2_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_fp8_e5m2_t.
 *
 * @override_id asc_loadalign_unpack__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous fp8_e4m3fn_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_fp8_e4m3fn_t.
 *
 * @override_id asc_loadalign_unpack__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous int16_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_int16_t.
 *
 * @override_id asc_loadalign_unpack__PUi16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_unpack(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous uint16_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_uint16_t.
 *
 * @override_id asc_loadalign_unpack__PUu16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_unpack(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous half elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_half.
 *
 * @override_id asc_loadalign_unpack__PUf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_unpack(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous bfloat16_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_bfloat16_t.
 *
 * @override_id asc_loadalign_unpack__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_unpack(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous int32_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_int32_t.
 *
 * @override_id asc_loadalign_unpack__PUi32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_unpack(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous uint32_t elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_uint32_t.
 *
 * @override_id asc_loadalign_unpack__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_unpack(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value form: reads VL/2 contiguous float elements from a 32-byte-aligned UB src, appends a zero after each source element, and returns the unpacked VL-length vector_float.
 *
 * @override_id asc_loadalign_unpack__PUf32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_unpack(__ubuf__ float* src);

// ========== asc_loadalign_unpack4 ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous int4b_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_int4x2_t register.
 *
 * @override_id asc_loadalign_unpack4__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack4(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous int8_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_int8_t register.
 *
 * @override_id asc_loadalign_unpack4__PUi8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_unpack4(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous uint8_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_uint8_t register.
 *
 * @override_id asc_loadalign_unpack4__PUu8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_unpack4(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous fp4x2_e2m1_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_fp4x2_e2m1_t register.
 *
 * @override_id asc_loadalign_unpack4__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack4(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous fp4x2_e1m2_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_fp4x2_e1m2_t register.
 *
 * @override_id asc_loadalign_unpack4__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack4(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous hifloat8_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_hifloat8_t register.
 *
 * @override_id asc_loadalign_unpack4__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack4(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous fp8_e8m0_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_fp8_e8m0_t register.
 *
 * @override_id asc_loadalign_unpack4__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack4(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous fp8_e5m2_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_fp8_e5m2_t register.
 *
 * @override_id asc_loadalign_unpack4__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack4(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/4 contiguous fp8_e4m3fn_t elements from a 32-byte-aligned UB src, pads each with three zeros, and returns the unpacked VL-length vector_fp8_e4m3fn_t register.
 *
 * @override_id asc_loadalign_unpack4__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack4(__ubuf__ fp8_e4m3fn_t* src);

// ========== asc_loadalign_upsample ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int4b_t elements from the 32-byte aligned UB src and returns a vector_int4x2_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUi4x2
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_upsample(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int8_t elements from the 32-byte aligned UB src and returns a vector_int8_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUi8
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_upsample(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 uint8_t elements from the 32-byte aligned UB src and returns a vector_uint8_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUu8
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_upsample(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp4x2_e2m1_t elements from the 32-byte aligned UB src and returns a vector_fp4x2_e2m1_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUf4e2m1x2
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_upsample(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp4x2_e1m2_t elements from the 32-byte aligned UB src and returns a vector_fp4x2_e1m2_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUf4e1m2x2
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_upsample(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 hifloat8_t elements from the 32-byte aligned UB src and returns a vector_hifloat8_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUhf8
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_upsample(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e8m0_t elements from the 32-byte aligned UB src and returns a vector_fp8_e8m0_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUf8e8m0
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_upsample(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e5m2_t elements from the 32-byte aligned UB src and returns a vector_fp8_e5m2_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUf8e5m2
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_upsample(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e4m3fn_t elements from the 32-byte aligned UB src and returns a vector_fp8_e4m3fn_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUf8e4m3
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_upsample(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int16_t elements from the 32-byte aligned UB src and returns a vector_int16_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUi16
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_upsample(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 uint16_t elements from the 32-byte aligned UB src and returns a vector_uint16_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUu16
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_upsample(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 half elements from the 32-byte aligned UB src and returns a vector_half register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUf16
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_upsample(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 bfloat16_t elements from the 32-byte aligned UB src and returns a vector_bfloat16_t register holding the result with each element repeated twice.
 *
 * @override_id asc_loadalign_upsample__PUbf16
 * @archs 3510
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_upsample(__ubuf__ bfloat16_t* src);

// ========== asc_loadalign_datablock_strided ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned int4x2 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUi4x2_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_datablock_strided(
    __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned int8 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUi8_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_datablock_strided(
    __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned uint8 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUu8_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_datablock_strided(
    __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned fp4x2_e2m1 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUf4e2m1x2_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned fp4x2_e1m2 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUf4e1m2x2_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned hifloat8 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUhf8_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_datablock_strided(
    __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned fp8_e8m0 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUf8e8m0_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned fp8_e5m2 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUf8e5m2_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned fp8_e4m3fn UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUf8e4m3_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned int16 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUi16_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_datablock_strided(
    __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned uint16 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUu16_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_datablock_strided(
    __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned half UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUf16_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_half asc_loadalign_datablock_strided(
    __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned bfloat16 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUbf16_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_datablock_strided(
    __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned int32 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUi32_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_datablock_strided(
    __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned uint32 UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUu32_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_datablock_strided(
    __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 8 DataBlocks of 32 bytes from 32-byte-aligned float UB src using block_stride and repeat_stride (in 32-byte units) with mask, and returns the result vector register.
 *
 * @override_id asc_loadalign_datablock_strided__PUf32_u16_u16_vb
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_float asc_loadalign_datablock_strided(
    __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

// ========== return mask register APIs ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value aligned mask load: reads VL/8 from a 32-byte aligned UB src and returns a vector_bool mask register, so the caller must update src manually between calls.
 *
 * @override_id asc_loadalign_mask__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐，搬入VL/8长度数据。
 */
__simd_callee__ inline vector_bool asc_loadalign_mask(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value 2x downsample mask load: reads VL/4 from a 32-byte aligned UB src, keeps even-index bits, and returns the VL/8 mask as vector_bool.
 *
 * @override_id asc_loadalign_mask_downsample__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐，读取VL/4长度数据。
 */
__simd_callee__ inline vector_bool asc_loadalign_mask_downsample(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Return-value 2x upsample mask load: reads VL/16 from a 16-byte aligned UB src, duplicates each bit twice, and returns the VL/8 mask as vector_bool.
 *
 * @override_id asc_loadalign_mask_upsample__PUu32
 * @archs 3510
 * @param [in] src 源UB地址，实际读取地址必须按16字节对齐，读取VL/16长度数据。
 */
__simd_callee__ inline vector_bool asc_loadalign_mask_upsample(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvu64_PUu64
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src);

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src);

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int8_t elements from the 32-byte aligned UB src into vector_int8_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 uint8_t elements from the 32-byte aligned UB src into vector_uint8_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp4x2_e2m1_t elements from the 32-byte aligned UB src into vector_fp4x2_e2m1_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp4x2_e1m2_t elements from the 32-byte aligned UB src into vector_fp4x2_e1m2_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int4b_t elements from the 32-byte aligned UB src into vector_int4x2_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e8m0_t elements from the 32-byte aligned UB src into vector_fp8_e8m0_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e5m2_t elements from the 32-byte aligned UB src into vector_fp8_e5m2_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e4m3fn_t elements from the 32-byte aligned UB src into vector_fp8_e4m3fn_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 hifloat8_t elements from the 32-byte aligned UB src into vector_hifloat8_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int16_t elements from the 32-byte aligned UB src into vector_int16_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 uint16_t elements from the 32-byte aligned UB src into vector_uint16_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 half elements from the 32-byte aligned UB src into vector_half dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 bfloat16_t elements from the 32-byte aligned UB src into vector_bfloat16_t dst, repeating each element twice to fill VL elements; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from a 32-byte-aligned UB src, keeps even-index elements, and stores them in the vector register; the user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int8_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint8_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp4x2_e2m1_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp4x2_e1m2_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int4b_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e8m0_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e5m2_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e4m3fn_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous hifloat8_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int16_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint16_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous half elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous bfloat16_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int32_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint32_t elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous float elements from a 32-byte-aligned UB src and inserts a zero after each source element; the user updates src.
 *
 * @override_id asc_loadalign_unpack__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL int8_t from the 32-byte aligned UB src into vector_int8_t dst0 (even elements) and vector_int8_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvi8_Rvi8_PUi8
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL uint8_t from the 32-byte aligned UB src into vector_uint8_t dst0 (even elements) and vector_uint8_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvu8_Rvu8_PUu8
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL fp4x2_e2m1_t from the 32-byte aligned UB src into vector_fp4x2_e2m1_t dst0 (even elements) and vector_fp4x2_e2m1_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvf4e2m1x2_Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL fp4x2_e1m2_t from the 32-byte aligned UB src into vector_fp4x2_e1m2_t dst0 (even elements) and vector_fp4x2_e1m2_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvf4e1m2x2_Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL int4b_t from the 32-byte aligned UB src into vector_int4x2_t dst0 (even elements) and vector_int4x2_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvi4x2_Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL fp8_e8m0_t from the 32-byte aligned UB src into vector_fp8_e8m0_t dst0 (even elements) and vector_fp8_e8m0_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvf8e8m0_Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL fp8_e5m2_t from the 32-byte aligned UB src into vector_fp8_e5m2_t dst0 (even elements) and vector_fp8_e5m2_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvf8e5m2_Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL fp8_e4m3fn_t from the 32-byte aligned UB src into vector_fp8_e4m3fn_t dst0 (even elements) and vector_fp8_e4m3fn_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvf8e4m3_Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL hifloat8_t from the 32-byte aligned UB src into vector_hifloat8_t dst0 (even elements) and vector_hifloat8_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvhf8_Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL int16_t from the 32-byte aligned UB src into vector_int16_t dst0 (even elements) and vector_int16_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvi16_Rvi16_PUi16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL uint16_t from the 32-byte aligned UB src into vector_uint16_t dst0 (even elements) and vector_uint16_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvu16_Rvu16_PUu16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL half from the 32-byte aligned UB src into vector_half dst0 (even elements) and vector_half dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvf16_Rvf16_PUf16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL bfloat16_t from the 32-byte aligned UB src into vector_bfloat16_t dst0 (even elements) and vector_bfloat16_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvbf16_Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL int32_t from the 32-byte aligned UB src into vector_int32_t dst0 (even elements) and vector_int32_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvi32_Rvi32_PUi32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL uint32_t from the 32-byte aligned UB src into vector_uint32_t dst0 (even elements) and vector_uint32_t dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvu32_Rvu32_PUu32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load: reads 2×VL float from the 32-byte aligned UB src into vector_float dst0 (even elements) and vector_float dst1 (odd elements).
 *
 * @override_id asc_loadalign_deintlv__Rvf32_Rvf32_PUf32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0, vector_float& dst1, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu64_PUu64_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int8_t elements from src plus int32_t offset (element units) into vector_int8_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 uint8_t elements from src plus int32_t offset (element units) into vector_uint8_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp4x2_e2m1_t elements from src plus int32_t offset (element units) into vector_fp4x2_e2m1_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp4x2_e1m2_t elements from src plus int32_t offset (element units) into vector_fp4x2_e1m2_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int4b_t elements from src plus int32_t offset (element units) into vector_int4x2_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e8m0_t elements from src plus int32_t offset (element units) into vector_fp8_e8m0_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e5m2_t elements from src plus int32_t offset (element units) into vector_fp8_e5m2_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 fp8_e4m3fn_t elements from src plus int32_t offset (element units) into vector_fp8_e4m3fn_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 hifloat8_t elements from src plus int32_t offset (element units) into vector_hifloat8_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 int16_t elements from src plus int32_t offset (element units) into vector_int16_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 uint16_t elements from src plus int32_t offset (element units) into vector_uint16_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 half elements from src plus int32_t offset (element units) into vector_half dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/2 bfloat16_t elements from src plus int32_t offset (element units) into vector_bfloat16_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from 32-byte-aligned UB src plus an int32_t element offset, keeps even-index elements; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int8_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint8_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp4x2_e2m1_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp4x2_e1m2_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int4b_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e8m0_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e5m2_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e4m3fn_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous hifloat8_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int16_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint16_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous half elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous bfloat16_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int32_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint32_t elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous float elements from 32-byte-aligned UB src plus an int32_t element offset, inserting a zero after each; src is not auto-updated.
 *
 * @override_id asc_loadalign_unpack__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL int8_t into vector_int8_t dst0 (even elements) and vector_int8_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi8_Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL uint8_t into vector_uint8_t dst0 (even elements) and vector_uint8_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvu8_Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL fp4x2_e2m1_t into vector_fp4x2_e2m1_t dst0 (even elements) and vector_fp4x2_e2m1_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf4e2m1x2_Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL fp4x2_e1m2_t into vector_fp4x2_e1m2_t dst0 (even elements) and vector_fp4x2_e1m2_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf4e1m2x2_Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL int4b_t into vector_int4x2_t dst0 (even elements) and vector_int4x2_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi4x2_Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL fp8_e8m0_t into vector_fp8_e8m0_t dst0 (even elements) and vector_fp8_e8m0_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf8e8m0_Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL fp8_e5m2_t into vector_fp8_e5m2_t dst0 (even elements) and vector_fp8_e5m2_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf8e5m2_Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL fp8_e4m3fn_t into vector_fp8_e4m3fn_t dst0 (even elements) and vector_fp8_e4m3fn_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf8e4m3_Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL hifloat8_t into vector_hifloat8_t dst0 (even elements) and vector_hifloat8_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvhf8_Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL int16_t into vector_int16_t dst0 (even elements) and vector_int16_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi16_Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL uint16_t into vector_uint16_t dst0 (even elements) and vector_uint16_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvu16_Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL half into vector_half dst0 (even elements) and vector_half dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf16_Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL bfloat16_t into vector_bfloat16_t dst0 (even elements) and vector_bfloat16_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvbf16_Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL int32_t into vector_int32_t dst0 (even elements) and vector_int32_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi32_Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL uint32_t into vector_uint32_t dst0 (even elements) and vector_uint32_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvu32_Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, int32_t element offset: reads 2×VL float into vector_float dst0 (even elements) and vector_float dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf32_Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL int8_t elements from the 32-byte aligned UB src into vector_int8_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL uint8_t elements from the 32-byte aligned UB src into vector_uint8_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL fp4x2_e2m1_t elements from the 32-byte aligned UB src into vector_fp4x2_e2m1_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL fp4x2_e1m2_t elements from the 32-byte aligned UB src into vector_fp4x2_e1m2_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL fp8_e8m0_t elements from the 32-byte aligned UB src into vector_fp8_e8m0_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL int4b_t elements from the 32-byte aligned UB src into vector_int4x2_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL fp8_e5m2_t elements from the 32-byte aligned UB src into vector_fp8_e5m2_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL fp8_e4m3fn_t elements from the 32-byte aligned UB src into vector_fp8_e4m3fn_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL hifloat8_t elements from the 32-byte aligned UB src into vector_hifloat8_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL int16_t elements from the 32-byte aligned UB src into vector_int16_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL uint16_t elements from the 32-byte aligned UB src into vector_uint16_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL half elements from the 32-byte aligned UB src into vector_half dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL bfloat16_t elements from the 32-byte aligned UB src into vector_bfloat16_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL int32_t elements from the 32-byte aligned UB src into vector_int32_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL uint32_t elements from the 32-byte aligned UB src into vector_uint32_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL float elements from the 32-byte aligned UB src into vector_float dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL int64_t elements from the 32-byte aligned UB src into vector_int64_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvi64_RPUi64_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int64_t& dst, __ubuf__ int64_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL uint64_t elements from the 32-byte aligned UB src into vector_uint64_t dst, then advances src by int32_t offset elements.
 *
 * @override_id asc_loadalign_postupdate__Rvu64_RPUu64_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_uint64_t& dst, __ubuf__ uint64_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_int8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_uint8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_fp4x2_e2m1_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_fp4x2_e1m2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_fp8_e8m0_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_int4x2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_fp8_e5m2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_fp8_e4m3fn_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_hifloat8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_int16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_uint16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_half dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample load: reads VL/2 from a 32-byte aligned UB src, duplicates each element twice to vector_bfloat16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_int8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_uint8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_fp4x2_e2m1_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_fp4x2_e1m2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_fp8_e8m0_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_int4x2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_fp8_e5m2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_fp8_e4m3fn_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_hifloat8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_int16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_uint16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_half dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample load: reads 2*VL (512B) from a 32-byte aligned UB src, writes even-index elements to vector_bfloat16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 int8 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 uint8 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 fp4x2_e2m1 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 fp4x2_e1m2 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 fp8_e8m0 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 int4x2 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 fp8_e5m2 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 fp8_e4m3fn elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 hifloat8 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 int16 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 uint16 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 half elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 bfloat16 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 int32 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 uint32 elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads VL/2 float elements from 32-byte-aligned UB src, inserts one zero-valued element after each source element into dst, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_unpack_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL int8 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvi8_Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL uint8 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvu8_Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL fp4x2_e2m1 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvf4e2m1x2_Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL fp4x2_e1m2 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvf4e1m2x2_Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL fp8_e8m0 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvf8e8m0_Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL int4x2 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvi4x2_Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL fp8_e5m2 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvf8e5m2_Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL fp8_e4m3fn elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvf8e4m3_Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL hifloat8 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvhf8_Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL int16 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvi16_Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL uint16 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvu16_Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL half elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvf16_Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_half& dst0, vector_half& dst1, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL bfloat16 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvbf16_Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL int32 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvi32_Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL uint32 elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvu32_Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads 2xVL float elements from 32-byte-aligned UB src, writes even-index elements to dst0 and odd-index elements to dst1, then post-updates src by offset elements (void return).
 *
 * @override_id asc_loadalign_deintlv_postupdate__Rvf32_Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_float& dst0, vector_float& dst1, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int8_t& dst, __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int16_t& dst, __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int32_t& dst, __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int64_t& dst, __ubuf__ int64_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_half& dst, __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using a non-contiguous DataBlock stride without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。本模式不支持`vector_bool`目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_float& dst, __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_int8_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvi8_RPUi8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_uint8_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvu8_RPUu8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_int16_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvi16_RPUi16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_uint16_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvu16_RPUu16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_int32_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvi32_RPUi32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_uint32_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvu32_RPUu32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_int64_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvi64_RPUi64_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int64_t& dst, __ubuf__ int64_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_bfloat16_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvbf16_RPUbf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_fp8_e4m3fn_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvf8e4m3_RPUf8e4m3_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_hifloat8_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvhf8_RPUhf8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_fp8_e5m2_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvf8e5m2_RPUf8e5m2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_fp8_e8m0_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvf8e8m0_RPUf8e8m0_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_fp4x2_e1m2_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_fp4x2_e2m1_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_int4x2_t dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvi4x2_RPUi4x2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_half dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvf16_RPUf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_half& dst, __ubuf__ half*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update non-contiguous load: fetches 8 DataBlocks (32 bytes each) from src into vector_float dst using block_stride and mask, then advances src by repeat_stride × 32 bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvf32_RPUf32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。 &bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。 &bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_float& dst, __ubuf__ float*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvi64_PUi64_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvu64_PUu64_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 fp4x2_e1m2_t elements from src plus offset into vector_fp4x2_e1m2_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 fp4x2_e2m1_t elements from src plus offset into vector_fp4x2_e2m1_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 int4b_t elements from src plus offset into vector_int4x2_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 uint8_t elements from src plus offset into vector_uint8_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 int8_t elements from src plus offset into vector_int8_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 fp8_e4m3fn_t elements from src plus offset into vector_fp8_e4m3fn_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 hifloat8_t elements from src plus offset into vector_hifloat8_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 fp8_e5m2_t elements from src plus offset into vector_fp8_e5m2_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 fp8_e8m0_t elements from src plus offset into vector_fp8_e8m0_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 uint16_t elements from src plus offset into vector_uint16_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 int16_t elements from src plus offset into vector_int16_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 bfloat16_t elements from src plus offset into vector_bfloat16_t dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/2 half elements from src plus offset into vector_half dst, repeating each element twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL downsampled elements to dst: reads 2*VL (512 bytes) from UB src at an addr_reg element offset from asc_update_addr_reg, keeps even-index elements; for Hardware Loop.
 *
 * @override_id asc_loadalign_downsample__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL fp4x2_e1m2_t into vector_fp4x2_e1m2_t dst0 (even elements) and vector_fp4x2_e1m2_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf4e1m2x2_Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL fp4x2_e2m1_t into vector_fp4x2_e2m1_t dst0 (even elements) and vector_fp4x2_e2m1_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf4e2m1x2_Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL int4b_t into vector_int4x2_t dst0 (even elements) and vector_int4x2_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi4x2_Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL uint8_t into vector_uint8_t dst0 (even elements) and vector_uint8_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvu8_Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL int8_t into vector_int8_t dst0 (even elements) and vector_int8_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi8_Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL fp8_e4m3fn_t into vector_fp8_e4m3fn_t dst0 (even elements) and vector_fp8_e4m3fn_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf8e4m3_Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL hifloat8_t into vector_hifloat8_t dst0 (even elements) and vector_hifloat8_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvhf8_Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL fp8_e5m2_t into vector_fp8_e5m2_t dst0 (even elements) and vector_fp8_e5m2_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf8e5m2_Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL fp8_e8m0_t into vector_fp8_e8m0_t dst0 (even elements) and vector_fp8_e8m0_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf8e8m0_Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL uint16_t into vector_uint16_t dst0 (even elements) and vector_uint16_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvu16_Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL int16_t into vector_int16_t dst0 (even elements) and vector_int16_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi16_Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL bfloat16_t into vector_bfloat16_t dst0 (even elements) and vector_bfloat16_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvbf16_Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL half into vector_half dst0 (even elements) and vector_half dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf16_Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL uint32_t into vector_uint32_t dst0 (even elements) and vector_uint32_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvu32_Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL int32_t into vector_int32_t dst0 (even elements) and vector_int32_t dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvi32_Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Deinterleave load, addr_reg offset: reads 2×VL float into vector_float dst0 (even elements) and vector_float dst1 (odd elements); src not updated.
 *
 * @override_id asc_loadalign_deintlv__Rvf32_Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp4x2_e1m2_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp4x2_e2m1_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int4b_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint8_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int8_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e4m3fn_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous hifloat8_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e5m2_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous fp8_e8m0_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint16_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int16_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous half elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous bfloat16_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous uint32_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous int32_t elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes unpacked VL elements to dst: reads VL/2 contiguous float elements from UB src at an addr_reg element offset from asc_update_addr_reg, inserting a zero after each; for Hardware Loop.
 *
 * @override_id asc_loadalign_unpack__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst in contiguous order without changing its data format.
 *
 * @override_id asc_loadalign__Rvb_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，`dtype`必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvb_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Loads data from aligned UB storage into dst using an immediate offset without changing its data format.
 *
 * @override_id asc_loadalign__Rvb_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/16 from the 16-byte aligned uint32_t UB src into vector_bool dst, duplicating each bit twice to fill VL/8 mask bits; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvb_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load: reads VL/16 from the 16-byte aligned uint32_t UB src at an int32_t byte offset into vector_bool dst, duplicating each bit twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvb_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief 2x upsample load with addr_reg offset: reads VL/16 from the 16-byte aligned uint32_t UB src into vector_bool dst, duplicating each bit twice; src is not updated.
 *
 * @override_id asc_loadalign_upsample__Rvb_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL/8 mask data to vector_bool dst: reads VL/4 (64 bytes) from a 32-byte-aligned uint32_t UB src, keeps even-index bits, and stores them in the mask register; user updates src.
 *
 * @override_id asc_loadalign_downsample__Rvb_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器，仅无返回值类型接口包含该参数。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL/8 mask data to vector_bool dst: reads VL/4 (64 bytes) from a 32-byte-aligned uint32_t UB src plus a byte offset, keeps even-index bits; src is not auto-updated.
 *
 * @override_id asc_loadalign_downsample__Rvb_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Writes VL/8 mask data to vector_bool dst: reads VL/4 (64 bytes) from a uint32_t UB src at an addr_reg b32-mode element offset (asc_update_addr_reg), keeping even-index bits.
 *
 * @override_id asc_loadalign_downsample__Rvb_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移。 &bull; 当`dst`为矢量数据寄存器，单位为元素个数。 &bull; 当`dst`为掩码寄存器，单位为字节。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update aligned load: reads VL/8 from the 32-byte aligned uint32_t UB src into vector_bool dst, then advances src by int32_t offset bytes.
 *
 * @override_id asc_loadalign_postupdate__Rvb_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x upsample mask load: reads VL/16 from a 16-byte aligned UB src, duplicates each bit twice into vector_bool dst, then advances src by offset bytes.
 *
 * @override_id asc_loadalign_upsample_postupdate__Rvb_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址。 &bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。 &bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update 2x downsample mask load: reads VL/4 (64B) from a 32-byte aligned UB src, keeps even-index bits in vector_bool dst, then advances src by offset bytes.
 *
 * @override_id asc_loadalign_downsample_postupdate__Rvb_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。 &bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。 &bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。 &bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。 &bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float output-parameter overload: reads a single element from the dtype-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_elem__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float immediate-offset overload: reads a single element from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_elem__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float addr_reg offset overload: reads a single element from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_elem__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous int8_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_int8_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous uint8_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_uint8_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp4x2_e2m1_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_fp4x2_e2m1_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp4x2_e1m2_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_fp4x2_e1m2_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous int4b_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_int4x2_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e8m0_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_fp8_e8m0_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e5m2_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_fp8_e5m2_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e4m3fn_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_fp8_e4m3fn_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous hifloat8_t elements from a 32-byte-aligned UB src, inserting three zeros after each, into vector_hifloat8_t dst; caller updates src.
 *
 * @override_id asc_loadalign_unpack4__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous int8_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_int8_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous uint8_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_uint8_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp4x2_e2m1_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_fp4x2_e2m1_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp4x2_e1m2_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_fp4x2_e1m2_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous int4b_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_int4x2_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e8m0_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_fp8_e8m0_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e5m2_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_fp8_e5m2_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e4m3fn_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_fp8_e4m3fn_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous hifloat8_t elements from UB src plus an int32_t element offset, inserting three zeros after each, into vector_hifloat8_t dst; src is not updated.
 *
 * @override_id asc_loadalign_unpack4__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp4x2_e1m2_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_fp4x2_e1m2_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp4x2_e2m1_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_fp4x2_e2m1_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous int4b_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_int4x2_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous uint8_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_uint8_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous int8_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_int8_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e4m3fn_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_fp8_e4m3fn_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous hifloat8_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_hifloat8_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e5m2_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_fp8_e5m2_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Unpacks VL/4 contiguous fp8_e8m0_t elements from UB src plus an addr_reg element offset (from asc_update_addr_reg), inserting three zeros after each, into vector_fp8_e8m0_t dst.
 *
 * @override_id asc_loadalign_unpack4__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float output-parameter overload: reads one 32-byte DataBlock from the 32-byte-aligned UB address src into vector register dst and broadcasts it to all lanes; caller updates src.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float immediate-offset overload: reads one 32-byte DataBlock from src plus the int32_t offset in elements into vector register dst and broadcasts it to all lanes; src is not auto-updated.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e1m2_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp4x2_e2m1_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int4b_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint8_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int8_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e4m3fn_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief hifloat8_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e5m2_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief fp8_e8m0_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint16_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int16_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief bfloat16_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief half addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief uint32_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief int32_t addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief float addr_reg offset overload: reads one 32-byte DataBlock from src plus addr_reg offset (via asc_update_addr_reg) into dst and broadcasts it to all lanes; used in Hardware Loop.
 *
 * @override_id asc_loadalign_brc_datablock__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int16 elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint16 elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 half elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 bfloat16 elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int32 elements from 32-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint32 elements from 32-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 float elements from 32-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。仅无返回值类型接口包含该参数。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int16 elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint16 elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 half elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 bfloat16 elements from 16-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int32 elements from 32-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint32 elements from 32-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 float elements from 32-byte-aligned UB src, writes each broadcast element over one 32-byte DataBlock of dst, and leaves src for the user to update (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint16 elements starting at src plus an addr_reg offset from asc_update_addr_reg, writes each to a 32-byte DataBlock of dst; src not auto-updated (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int16 elements starting at src plus an addr_reg offset from asc_update_addr_reg, writes each to a 32-byte DataBlock of dst; src not auto-updated (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 bfloat16 elements starting at src plus an addr_reg offset from asc_update_addr_reg, writes each to a 32-byte DataBlock of dst; src not auto-updated (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 half elements starting at src plus an addr_reg offset from asc_update_addr_reg, writes each to a 32-byte DataBlock of dst; src not auto-updated (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 uint32 elements starting at src plus an addr_reg offset from asc_update_addr_reg, writes each to a 32-byte DataBlock of dst; src not auto-updated (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 int32 elements starting at src plus an addr_reg offset from asc_update_addr_reg, writes each to a 32-byte DataBlock of dst; src not auto-updated (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Broadcast-loads 8 float elements starting at src plus an addr_reg offset from asc_update_addr_reg, writes each to a 32-byte DataBlock of dst; src not auto-updated (void return).
 *
 * @override_id asc_loadalign_brc_elem2datablock__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset 相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one int8 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one uint8 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one fp4x2_e2m1 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one fp4x2_e1m2 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one fp8_e8m0 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one int4x2 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one fp8_e5m2 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one fp8_e4m3fn element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one hifloat8 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one int16 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one uint16 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one half element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one bfloat16 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one int32 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one uint32 element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one float element from a dtype-aligned UB src and broadcasts it over the whole VL of dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_int8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_uint8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_fp4x2_e2m1_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_fp4x2_e1m2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_fp8_e8m0_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_int4x2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_fp8_e5m2_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_fp8_e4m3fn_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update unpack load: reads VL/4 from a 32-byte aligned UB src, pads each element with three zero elements to vector_hifloat8_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_unpack4_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of int8 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of uint8 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of fp4x2_e2m1 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of fp4x2_e1m2 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of fp8_e8m0 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of int4x2 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of fp8_e5m2 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of fp8_e4m3fn dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of hifloat8 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of int16 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of uint16 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of half dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of bfloat16 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of int32 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of uint32 dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Reads one 32-byte DataBlock from 32-byte-aligned UB src and broadcasts it over the whole VL of float dst, then post-updates src by offset x sizeof(dtype) bytes (void return).
 *
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update broadcast load: reads 8 elements from a 16/32-byte aligned UB src and broadcasts each into one 32-byte DataBlock of vector_int16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update broadcast load: reads 8 elements from a 16/32-byte aligned UB src and broadcasts each into one 32-byte DataBlock of vector_uint16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update broadcast load: reads 8 elements from a 16/32-byte aligned UB src and broadcasts each into one 32-byte DataBlock of vector_half dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update broadcast load: reads 8 elements from a 16/32-byte aligned UB src and broadcasts each into one 32-byte DataBlock of vector_bfloat16_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update broadcast load: reads 8 elements from a 16/32-byte aligned UB src and broadcasts each into one 32-byte DataBlock of vector_int32_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update broadcast load: reads 8 elements from a 16/32-byte aligned UB src and broadcasts each into one 32-byte DataBlock of vector_uint32_t dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Post-update broadcast load: reads 8 elements from a 16/32-byte aligned UB src and broadcasts each into one 32-byte DataBlock of vector_float dst, then advances src by offset elements.
 *
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in,out] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_int8_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_uint8_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_fp4x2_e2m1_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_fp4x2_e1m2_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_int4x2_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_fp8_e8m0_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_fp8_e5m2_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_fp8_e4m3fn_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src and unpacks them into VL-length vector_hifloat8_t dst with zeros inserted; caller updates src; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_int8_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvi8_PUi8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_uint8_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvu8_PUu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_fp4x2_e2m1_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_fp4x2_e1m2_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_int4x2_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvi4x2_PUi4x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_fp8_e8m0_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_fp8_e5m2_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_fp8_e4m3fn_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_hifloat8_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvhf8_PUhf8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_int16_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_uint16_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_half dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_bfloat16_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_int32_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_uint32_t dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src and broadcasts it to the whole VL register vector_float dst; caller updates src; replaced by asc_loadalign_brc_datablock.
 *
 * @override_id asc_loadalign_brc_v2__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src and broadcasts each element into a 32-byte DataBlock of vector_int16_t dst; caller updates src; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvi16_PUi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src and broadcasts each element into a 32-byte DataBlock of vector_uint16_t dst; caller updates src; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvu16_PUu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src and broadcasts each element into a 32-byte DataBlock of vector_half dst; caller updates src; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvf16_PUf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src and broadcasts each element into a 32-byte DataBlock of vector_bfloat16_t dst; caller updates src; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvbf16_PUbf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src and broadcasts each element into a 32-byte DataBlock of vector_int32_t dst; caller updates src; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvi32_PUi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src and broadcasts each element into a 32-byte DataBlock of vector_uint32_t dst; caller updates src; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvu32_PUu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src and broadcasts each element into a 32-byte DataBlock of vector_float dst; caller updates src; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvf32_PUf32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_int8_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_uint8_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_fp4x2_e2m1_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_fp4x2_e1m2_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_int4x2_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_fp8_e8m0_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_fp8_e5m2_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_fp8_e4m3fn_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus int32_t element offset and unpacks them into VL-length vector_hifloat8_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_int8_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi8_PUi8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_uint8_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvu8_PUu8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_fp4x2_e2m1_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_fp4x2_e1m2_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_int4x2_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_fp8_e8m0_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_fp8_e5m2_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_fp8_e4m3fn_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_hifloat8_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvhf8_PUhf8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_int16_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_uint16_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_half dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_bfloat16_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_int32_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_uint32_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus int32_t element offset and broadcasts it to the whole VL register vector_float dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus int32_t offset and broadcasts each element into a 32-byte DataBlock of vector_int16_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvi16_PUi16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus int32_t offset and broadcasts each element into a 32-byte DataBlock of vector_uint16_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvu16_PUu16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus int32_t offset and broadcasts each element into a 32-byte DataBlock of vector_half dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvf16_PUf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus int32_t offset and broadcasts each element into a 32-byte DataBlock of vector_bfloat16_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvbf16_PUbf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus int32_t offset and broadcasts each element into a 32-byte DataBlock of vector_int32_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvi32_PUi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus int32_t offset and broadcasts each element into a 32-byte DataBlock of vector_uint32_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvu32_PUu32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus int32_t offset and broadcasts each element into a 32-byte DataBlock of vector_float dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvf32_PUf32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 int8 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 uint8 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 fp4x2_e2m1 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 fp4x2_e1m2 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 fp8_e8m0 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 int4x2 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 fp8_e5m2 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 fp8_e4m3fn elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated UNPACKV2: reads VL/4 hifloat8 elements as u8 from UB src, unpacks to VL u32 values with zero interleaving in dst, then post-updates src; use asc_loadalign_unpack4_postupdate.
 *
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate_v2(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from int8 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi8_RPUi8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from uint8 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvu8_RPUu8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from fp4x2_e2m1 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from fp4x2_e1m2 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from fp8_e8m0 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from int4x2 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from fp8_e5m2 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from fp8_e4m3fn UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from hifloat8 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from int16 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from uint16 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from half UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(vector_half& dst, __ubuf__ half*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from bfloat16 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from int32 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from uint32 UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV2: loads one 32-byte DataBlock from float UB src, broadcasts it to dst across VL, then post-updates src by offset elements; use asc_loadalign_brc_datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v2(vector_float& dst, __ubuf__ float*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV3: loads VL/DataBlock int16 elements from UB src, broadcasts each to one 32-byte DataBlock of dst, then post-updates src; use asc_loadalign_brc_elem2datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v3__Rvi16_RPUi16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV3: loads VL/DataBlock uint16 elements from UB src, broadcasts each to one 32-byte DataBlock of dst, then post-updates src; use asc_loadalign_brc_elem2datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v3__Rvu16_RPUu16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV3: loads VL/DataBlock half elements from UB src, broadcasts each to one 32-byte DataBlock of dst, then post-updates src; use asc_loadalign_brc_elem2datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v3__Rvf16_RPUf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(vector_half& dst, __ubuf__ half*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV3: loads VL/DataBlock bfloat16 elements from UB src, broadcasts each to one 32-byte DataBlock of dst, then post-updates src; use asc_loadalign_brc_elem2datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v3__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV3: loads VL/DataBlock int32 elements from UB src, broadcasts each to one 32-byte DataBlock of dst, then post-updates src; use asc_loadalign_brc_elem2datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v3__Rvi32_RPUi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV3: loads VL/DataBlock uint32 elements from UB src, broadcasts each to one 32-byte DataBlock of dst, then post-updates src; use asc_loadalign_brc_elem2datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v3__Rvu32_RPUu32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated BRCV3: loads VL/DataBlock float elements from UB src, broadcasts each to one 32-byte DataBlock of dst, then post-updates src; use asc_loadalign_brc_elem2datablock_postupdate.
 *
 * @override_id asc_loadalign_brc_postupdate_v3__Rvf32_RPUf32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in,out] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline void asc_loadalign_brc_postupdate_v3(vector_float& dst, __ubuf__ float*& src, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_fp4x2_e1m2_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_fp4x2_e2m1_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_int4x2_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_uint8_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_int8_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_fp8_e4m3fn_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_hifloat8_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_fp8_e5m2_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_unpack4)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/4 bytes as unsigned u8 from UB src plus addr_reg offset and unpacks them into VL-length vector_fp8_e8m0_t dst with zeros inserted; replaced by asc_loadalign_unpack4.
 *
 * @override_id asc_loadalign_unpack_v2__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_fp4x2_e1m2_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_fp4x2_e2m1_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_int4x2_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_uint8_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvu8_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_int8_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi8_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_fp8_e4m3fn_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_hifloat8_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvhf8_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_fp8_e5m2_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_fp8_e8m0_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_uint16_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_int16_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_bfloat16_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_half dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_uint32_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_int32_t dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: reads one 32-byte DataBlock from UB src plus addr_reg offset (asc_update_addr_reg) and broadcasts it to the whole VL register vector_float dst.
 *
 * @override_id asc_loadalign_brc_v2__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus addr_reg offset and broadcasts each element into a 32-byte DataBlock of vector_uint16_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvu16_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus addr_reg offset and broadcasts each element into a 32-byte DataBlock of vector_int16_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvi16_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus addr_reg offset and broadcasts each element into a 32-byte DataBlock of vector_bfloat16_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvbf16_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus addr_reg offset and broadcasts each element into a 32-byte DataBlock of vector_half dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvf16_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus addr_reg offset and broadcasts each element into a 32-byte DataBlock of vector_uint32_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvu32_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus addr_reg offset and broadcasts each element into a 32-byte DataBlock of vector_int32_t dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvi32_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_loadalign_brc_elem2datablock)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: loads VL/DataBlock bytes from UB src plus addr_reg offset and broadcasts each element into a 32-byte DataBlock of vector_float dst; replaced by asc_loadalign_brc_elem2datablock.
 *
 * @override_id asc_loadalign_brc_v3__Rvf32_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
__simd_callee__ inline void asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, addr_reg offset);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H
#endif
