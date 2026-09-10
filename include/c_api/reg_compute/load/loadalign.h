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
#include "impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h"
#endif

// ========== return-value load APIs ==========
// ========== return vector data register APIs ==========
// ========== asc_loadalign ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign(__ubuf__ float* src);

// ========== asc_loadalign_brc_datablock ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_datablock(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_brc_datablock(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_brc_datablock(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_datablock(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_datablock(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_datablock(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_datablock(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_brc_datablock(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_datablock(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_brc_datablock(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_datablock(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_brc_datablock(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_datablock(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_brc_datablock(__ubuf__ float* src);

// ========== asc_loadalign_brc_elem ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_elem(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_brc_elem(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_brc_elem(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_elem(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_elem(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_elem(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_elem(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_brc_elem(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_brc_elem(__ubuf__ float* src);

// ========== asc_loadalign_brc_elem2datablock ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem2datablock(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem2datablock(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_brc_elem2datablock(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem2datablock(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem2datablock(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem2datablock(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_brc_elem2datablock(__ubuf__ float* src);

// ========== asc_loadalign_downsample ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_downsample(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_downsample(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_downsample(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_downsample(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_downsample(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_downsample(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_downsample(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_downsample(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_downsample(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_downsample(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_downsample(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_downsample(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_downsample(__ubuf__ bfloat16_t* src);

// ========== asc_loadalign_unpack ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_unpack(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_unpack(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_unpack(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_unpack(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_unpack(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_unpack(__ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_unpack(__ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_unpack(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_float asc_loadalign_unpack(__ubuf__ float* src);

// ========== asc_loadalign_unpack4 ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack4(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_unpack4(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_unpack4(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack4(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack4(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack4(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack4(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack4(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack4(__ubuf__ fp8_e4m3fn_t* src);

// ========== asc_loadalign_upsample ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_upsample(__ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_upsample(__ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_upsample(__ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_upsample(__ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_upsample(__ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_upsample(__ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_upsample(__ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_upsample(__ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_upsample(__ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_upsample(__ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_upsample(__ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_half asc_loadalign_upsample(__ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_upsample(__ubuf__ bfloat16_t* src);

// ========== asc_loadalign_datablock_strided ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUi4x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int4x2_t asc_loadalign_datablock_strided(
    __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUi8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int8_t asc_loadalign_datablock_strided(
    __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUu8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_uint8_t asc_loadalign_datablock_strided(
    __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUf4e2m1x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUf4e1m2x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUhf8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_hifloat8_t asc_loadalign_datablock_strided(
    __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUf8e8m0_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUf8e5m2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUf8e4m3_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUi16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int16_t asc_loadalign_datablock_strided(
    __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUu16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_uint16_t asc_loadalign_datablock_strided(
    __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUf16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_half asc_loadalign_datablock_strided(
    __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUbf16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_bfloat16_t asc_loadalign_datablock_strided(
    __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUi32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_int32_t asc_loadalign_datablock_strided(
    __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUu32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_uint32_t asc_loadalign_datablock_strided(
    __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_datablock_strided__PUf32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址非连续搬入8个`DataBlock`，并通过函数返回值返回保存搬入结果的矢量数据寄存器。每个`DataBlock`的数据量为32字节，支持配置相邻数据块之间的地址步长和本次搬入的起始读取位置。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline vector_float asc_loadalign_datablock_strided(
    __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

// ========== return mask register APIs ==========
/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_mask__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/8长度数据，并通过函数返回值返回掩码寄存器。搬运过程中数据格式和内容保持不变。连续搬入时，需要在每次调用前手动更新源地址。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐，搬入VL/8长度数据。
 */
__simd_callee__ inline vector_bool asc_loadalign_mask(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_mask_downsample__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度数据，保留偶数下标bit，得到VL/8长度数据，并通过函数返回值返回掩码寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐，读取VL/4长度数据。
 */
__simd_callee__ inline vector_bool asc_loadalign_mask_downsample(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_mask_upsample__PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐的起始地址读取VL/16长度数据，将每个bit重复两次，得到VL/8长度数据，并通过函数返回值返回掩码寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按16字节对齐，读取VL/16长度数据。
 */
__simd_callee__ inline vector_bool asc_loadalign_mask_upsample(__ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi32_PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu32_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf32_PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi64_PUi64
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu64_PUu64
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
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
 * @override_id asc_loadalign_upsample__Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi32_PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu32_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf32_PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvi8_PUi8
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvu8_PUu8
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvhf8_PUhf8
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi8_PUi8
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu8_PUu8
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvhf8_PUhf8
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi16_PUi16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu16_PUu16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf16_PUf16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvbf16_PUbf16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi32_PUi32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu32_PUu32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf32_PUf32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvi16_PUi16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvu16_PUu16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvf16_PUf16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvbf16_PUbf16
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvi32_PUi32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvu32_PUu32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvf32_PUf32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi8_Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu8_Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf4e2m1x2_Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf4e1m2x2_Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi4x2_Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e8m0_Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e5m2_Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e4m3_Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvhf8_Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi16_Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu16_Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf16_Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvbf16_Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi32_Rvi32_PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu32_Rvu32_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf32_Rvf32_PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0, vector_float& dst1, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi32_PUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu32_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf32_PUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi64_PUi64_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu64_PUu64_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
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
 * @override_id asc_loadalign_upsample__Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi32_PUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu32_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf32_PUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvi8_PUi8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvu8_PUu8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi8_PUi8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu8_PUu8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi16_PUi16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu16_PUu16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf16_PUf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi32_PUi32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu32_PUu32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf32_PUf32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvi16_PUi16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvu16_PUu16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvf16_PUf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvi32_PUi32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvu32_PUu32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvf32_PUf32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi8_Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu8_Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf4e2m1x2_Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf4e1m2x2_Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi4x2_Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e8m0_Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e5m2_Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e4m3_Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvhf8_Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi16_Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu16_Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf16_Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvbf16_Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi32_Rvi32_PUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu32_Rvu32_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf32_Rvf32_PUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi64_RPUi64_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_int64_t& dst, __ubuf__ int64_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvu64_RPUu64_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
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
 * @override_id asc_loadalign_upsample_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补1个0，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_postupdate_v2__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_half& dst, __ubuf__ half*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvi32_RPUi32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvu32_RPUu32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v2__Rvf32_RPUf32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_float& dst, __ubuf__ float*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v3__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v3__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v3__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_half& dst, __ubuf__ half*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v3__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v3__Rvi32_RPUi32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v3__Rvu32_RPUu32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_postupdate_v3__Rvf32_RPUf32_i32
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_postupdate_v2请使用[asc_loadalign_brc_datablock_postupdate](../reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)，asc_loadalign_brc_postupdate_v3请使用[asc_loadalign_brc_elem2datablock_postupdate](../reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)，asc_loadalign_unpack_postupdate_v2请使用[asc_loadalign_unpack4_postupdate](../reg_compute/load/asc_loadalign_unpack4_postupdate.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量。
 */
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvi8_Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvu8_Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvf4e2m1x2_Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvf4e1m2x2_Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvf8e8m0_Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvi4x2_Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvf8e5m2_Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvf8e4m3_Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvhf8_Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvi16_Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvu16_Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvf16_Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_half& dst0, vector_half& dst1, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvbf16_Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvi32_Rvi32_RPUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvu32_Rvu32_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv_postupdate__Rvf32_Rvf32_RPUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_float& dst0, vector_float& dst1, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi8_PUi8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int8_t& dst, __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu8_PUu8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi16_PUi16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int16_t& dst, __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu16_PUu16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi32_PUi32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int32_t& dst, __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu32_PUu32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi64_PUi64_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int64_t& dst, __ubuf__ int64_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvbf16_PUbf16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvhf8_PUhf8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf16_PUf16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_half& dst, __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf32_PUf32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign(
    vector_float& dst, __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi8_RPUi8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvu8_RPUu8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi16_RPUi16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvu16_RPUu16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi32_RPUi32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvu32_RPUu32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi64_RPUi64_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int64_t& dst, __ubuf__ int64_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvbf16_RPUbf16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf8e4m3_RPUf8e4m3_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvhf8_RPUhf8_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf8e5m2_RPUf8e5m2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf8e8m0_RPUf8e8m0_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvi4x2_RPUi4x2_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf16_RPUf16_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_half& dst, __ubuf__ half*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvf32_RPUf32_u16_u16_vb
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] block_stride 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。
 * @param [in] repeat_stride Post Update步长，单位为32字节。接口完成本次搬入后，将`src`地址增加`repeat_stride × 32`字节。
 * @param [in] mask 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。
 */
__simd_callee__ inline void asc_loadalign_postupdate(
    vector_float& dst, __ubuf__ float*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu32_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi32_PUi32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvi64_PUi64_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvu64_PUu64_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvf32_PUf32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
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
 * @override_id asc_loadalign_upsample__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf4e1m2x2_Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf4e2m1x2_Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi4x2_Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu8_Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi8_Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e4m3_Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvhf8_Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e5m2_Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf8e8m0_Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu16_Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi16_Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvbf16_Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf16_Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvu32_Rvu32_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvi32_Rvi32_PUi32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_deintlv__Rvf32_Rvf32_PUf32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`。
 * @param [out] dst0 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的偶数下标元素，数据长度为VL。
 * @param [out] dst1 目的矢量数据寄存器。dtype必须与`src`一致，搬入源数据中的奇数下标元素，数据长度为VL。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvu32_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvi32_PUi32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack__Rvf32_PUf32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/2长度的连续数据，在每个源元素后补1个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvu8_PUu8_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvi8_PUi8_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_unpack_v2__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu8_PUu8_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi8_PUi8_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu16_PUu16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi16_PUi16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf16_PUf16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvu32_PUu32_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvi32_PUi32_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v2__Rvf32_PUf32_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvu16_PUu16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvi16_PUi16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvf16_PUf16_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvu32_PUu32_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvi32_PUi32_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_loadalign_brc_v3__Rvf32_PUf32_ar
 * @archs 3510
 * @brief **该接口已废弃。其中asc_loadalign_brc_v2请使用[asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)，asc_loadalign_brc_v3请使用[asc_loadalign_brc_elem2datablock](../reg_compute/load/asc_loadalign_brc_elem2datablock.md)，asc_loadalign_unpack_v2请使用[asc_loadalign_unpack4](../reg_compute/load/asc_loadalign_unpack4.md)实现此功能。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数，UB中32B对齐的起始地址。
 * @param [in] offset 地址寄存器，必须通过[asc_update_addr_reg](../reg_compute/reg_addr_reg/asc_update_addr_reg.md)接口生成，用于记录相对`src`起始地址的偏移，单位为元素个数。
 */
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvb_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvb_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign__Rvb_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并通过函数返回值返回或写入目的矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvb_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvb_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample__Rvb_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址。&bull; 当`dst`为矢量数据寄存器，实际读取地址必须按32字节对齐。&bull; 当`dst`为掩码寄存器，实际读取地址必须按16字节对齐。
 */
__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvb_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvb_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample__Rvb_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样，结果通过函数返回值返回或写入目的寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_postupdate__Rvb_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取数据，并搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`指针增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_postupdate(vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_upsample_postupdate__Rvb_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中满足对应对齐要求的起始地址读取连续数据并进行2倍上采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_downsample_postupdate__Rvb_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取连续数据并进行2倍下采样。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器或掩码寄存器。&bull; 当`dst`为矢量数据寄存器，dtype必须与`src`一致，搬入VL长度数据。&bull; 当`dst`为掩码寄存器，搬入VL/8长度数据。
 * @param [in] offset Post Update步长。接口从当前`src`地址完成搬入后更新`src`。&bull; 当`dst`为矢量数据寄存器时，单位为元素个数，`src`增加`offset × sizeof(dtype)`字节。&bull; 当`dst`为掩码寄存器时，单位为字节，`src`增加`offset`字节。
 */
__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi32_PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu32_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf32_PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi32_PUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu32_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf32_PUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvu32_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvi32_PUi32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem__Rvf32_PUf32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据通过函数返回值返回或写入目的矢量数据寄存器。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi8_PUi8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu8_PUu8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf4e2m1x2_PUf4e2m1x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf4e1m2x2_PUf4e1m2x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi4x2_PUi4x2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e8m0_PUf8e8m0
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e5m2_PUf8e5m2
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e4m3_PUf8e4m3
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvhf8_PUhf8
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi32_PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu32_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf32_PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi8_PUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu8_PUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf4e2m1x2_PUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf4e1m2x2_PUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi4x2_PUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e8m0_PUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e5m2_PUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e4m3_PUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvhf8_PUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi32_PUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu32_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf32_PUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf4e1m2x2_PUf4e1m2x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf4e2m1x2_PUf4e2m1x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi4x2_PUi4x2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu8_PUu8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi8_PUi8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e4m3_PUf8e4m3_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvhf8_PUhf8_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e5m2_PUf8e5m2_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf8e8m0_PUf8e8m0_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvu32_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvi32_PUi32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock__Rvf32_PUf32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvi16_PUi16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int16_t& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvu16_PUu16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvf16_PUf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvbf16_PUbf16
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvi32_PUi32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int32_t& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvu32_PUu32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvf32_PUf32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvi16_PUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvu16_PUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvf16_PUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvbf16_PUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvi32_PUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvu32_PUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvf32_PUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvu16_PUu16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvi16_PUi16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvbf16_PUbf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvf16_PUf16_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvu32_PUu32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvi32_PUi32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock__Rvf32_PUf32_ar
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到输出结果对应的一个`DataBlock`（32字节）中。结果通过函数返回值返回或写入目的矢量数据寄存器，搬运过程中数据格式和内容保持不变。
 * @param [in] src 源UB地址，实际读取地址必须满足对齐要求：b16类型必须按16字节对齐，b32类型必须按32字节对齐。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中按dtype对齐的起始地址读取一个元素，并将该元素广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_unpack4_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4长度的连续数据，在每个源元素后补3个值为0的元素，并将得到的VL长度数据写入目的矢量数据寄存器。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，每个源元素后补3个值为0的元素，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi8_RPUi8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvu8_RPUu8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf4e2m1x2_RPUf4e2m1x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf4e1m2x2_RPUf4e1m2x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf8e8m0_RPUf8e8m0_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi4x2_RPUi4x2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf8e5m2_RPUf8e5m2_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf8e4m3_RPUf8e4m3_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvhf8_RPUhf8_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_datablock_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中32字节对齐的起始地址读取一个`DataBlock`（32字节），并将该`DataBlock`广播到整个矢量数据寄存器。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvi16_RPUi16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvu16_RPUu16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvf16_RPUf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvbf16_RPUbf16_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvi32_RPUi32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvu32_RPUu32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @override_id asc_loadalign_brc_elem2datablock_postupdate__Rvf32_RPUf32_i32
 * @archs 3510
 * @brief 从Unified Buffer（UB）中16字节对齐（b16类型）或32字节对齐（b32类型）的起始地址连续读取8个元素，并将每个元素广播到目的矢量数据寄存器对应的一个`DataBlock`（32字节）中。搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新源地址指针。
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。
 * @param [in] offset Post Update步长，单位为元素个数。接口从当前`src`地址完成搬入后更新`src`，`src`增加`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADALIGN_H
#endif
