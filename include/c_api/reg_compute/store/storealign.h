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
#error "reg_compute/store/storealign.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREALIGN_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_STORE_STOREALIGN_H
#define INCLUDE_C_API_REG_COMPUTE_STORE_STOREALIGN_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/storealign_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi8_vi8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu8_vu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int16_t* dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUhf8_vhf8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf4e2m1x2_vf4e2m1x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf4e1m2x2_vf4e1m2x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi4x2_vi4x2_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int4b_t* dst, vector_int4x2_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ int16_t* dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ int64_t* dst, vector_int64_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint64_t* dst, vector_uint64_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ float* dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ half* dst, vector_half src0, vector_half src1);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1);

__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int4b_t* dst, vector_int4x2_t src0, vector_int4x2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi8_vi8_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst, vector_int8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu8_vu8_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_uint8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi16_vi16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int16_t* dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu16_vu16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu32_vu32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf16_vf16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf32_vf32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUbf16_vbf16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUhf8_vhf8_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf4e2m1x2_vf4e2m1x2_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf4e1m2x2_vf4e1m2x2_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi4x2_vi4x2_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int4b_t* dst, vector_int4x2_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int16_t* dst, vector_int16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int64_t* dst, vector_int64_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint64_t* dst, vector_uint64_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ half* dst, vector_half src0, vector_half src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int4b_t* dst, vector_int4x2_t src0, vector_int4x2_t src1, int32_t offset);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int8_t*& dst, vector_int8_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint8_t*& dst, vector_uint8_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ half*& dst, vector_half src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int8_t*& dst, vector_int8_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint8_t*& dst, vector_uint8_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ half*& dst, vector_half src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ float*& dst, vector_float src, int32_t offset);

__simd_callee__ inline void asc_storealign_1st_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int64_t*& dst, vector_int64_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint64_t*& dst, vector_uint64_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ half*& dst, vector_half src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi8_vi8_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst, vector_int8_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu8_vu8_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint8_t* dst, vector_uint8_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi16_vi16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu16_vu16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi32_vi32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu32_vu32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf16_vf16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf32_vf32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUbf16_vbf16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUhf8_vhf8_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf4e2m1x2_vf4e2m1x2_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf4e1m2x2_vf4e1m2x2_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi4x2_vi4x2_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int4b_t* dst, vector_int4x2_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src, addr_reg offset);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int64_t* dst, vector_int64_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint64_t* dst, vector_uint64_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ half* dst, vector_half src0, vector_half src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1, addr_reg offset);

__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int4b_t* dst, vector_int4x2_t src0, vector_int4x2_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi8_vi8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int8_t* dst, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu8_vu8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint8_t* dst, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi16_vi16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int16_t* dst, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu16_vu16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint16_t* dst, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUi32_vi32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int32_t* dst, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUu32_vu32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint32_t* dst, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUbf16_vbf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUhf8_vhf8_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf16_vf16_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ half* dst, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using a non-contiguous DataBlock stride, preserving unselected destination elements under mask.
 *
 * @override_id asc_storealign__PUf32_vf32_u16_u16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ float* dst, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int8_t*& dst, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint8_t*& dst, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ half*& dst, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ float*& dst, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset.
 *
 * @override_id asc_storealign__PUu32_vb_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src, addr_reg offset);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset.
 *
 * @override_id asc_storealign__PUu16_vb_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src, addr_reg offset);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset.
 *
 * @override_id asc_storealign__PUu8_vb_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src, addr_reg offset);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order.
 *
 * @override_id asc_storealign__PUu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order.
 *
 * @override_id asc_storealign__PUu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage in contiguous order.
 *
 * @override_id asc_storealign__PUu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset.
 *
 * @override_id asc_storealign__PUu32_vb_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset.
 *
 * @override_id asc_storealign__PUu16_vb_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores register data to aligned UB storage using an immediate offset.
 *
 * @override_id asc_storealign__PUu8_vb_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对dst起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset);

__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ float* dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

__simd_callee__ inline void asc_storealign_squeeze_status(__ubuf__ uint32_t* dst);

__simd_callee__ inline void asc_storealign_squeeze_status(__ubuf__ uint32_t* dst, int32_t offset);

__simd_callee__ inline void asc_storealign_squeeze_status_postupdate(__ubuf__ uint32_t*& dst, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter_postupdate)
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter_postupdate)
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter_postupdate)
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, addr_reg offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, addr_reg offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ float* dst_align32b, vector_float src, addr_reg offset, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREALIGN_H
#endif
