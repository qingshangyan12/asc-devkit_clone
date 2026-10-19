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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int8_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUi8_vi8
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint8_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUu8_vu8
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int16_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUi16_vi16
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint16_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUu16_vu16
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int32_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUi32_vi32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint32_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUu32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_half register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUf16_vf16
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_float register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUf32_vf32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_bfloat16_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUbf16_vbf16
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e4m3fn_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUf8e4m3_vf8e4m3
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_hifloat8_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUhf8_vhf8
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e5m2_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUf8e5m2_vf8e5m2
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e8m0_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUf8e8m0_vf8e8m0
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp4x2_e2m1_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUf4e2m1x2_vf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp4x2_e1m2_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUf4e1m2x2_vf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int4x2_t register to UB dst, which must be aligned to sizeof(dtype); returns void.
 *
 * @override_id asc_storealign_1st__PUi4x2_vi4x2
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int16_t elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUi16_vi16_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int16_t* dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint16_t elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUu16_vu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int32_t elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint32_t elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int64_t elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUi64_vi64_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int64_t* dst, vector_int64_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint64_t elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUu64_vu64_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint64_t* dst, vector_uint64_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_half elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUf16_vf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_float elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ float* dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_bfloat16_t elements and stores them contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_pack__PUbf16_vbf16_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int8_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUi8_vi8_vi8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint8_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUu8_vu8_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int16_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUi16_vi16_vi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint16_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUu16_vu16_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int32_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUi32_vi32_vi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint32_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUu32_vu32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_half registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUf16_vf16_vf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ half* dst, vector_half src0, vector_half src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_bfloat16_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUbf16_vbf16_vbf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e4m3fn_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUf8e4m3_vf8e4m3_vf8e4m3
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_hifloat8_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUhf8_vhf8_vhf8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e5m2_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUf8e5m2_vf8e5m2_vf8e5m2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e8m0_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUf8e8m0_vf8e8m0_vf8e8m0
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp4x2_e2m1_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUf4e2m1x2_vf4e2m1x2_vf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp4x2_e1m2_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUf4e1m2x2_vf4e1m2x2_vf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int4x2_t registers and stores the 2 x VL result contiguously to the 32-byte-aligned UB address dst; the caller updates dst.
 *
 * @override_id asc_storealign_intlv__PUi4x2_vi4x2_vi4x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int8_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUi8_vi8_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint8_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUu8_vu8_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int16_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUi16_vi16_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint16_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUu16_vu16_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int32_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUi32_vi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint32_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUu32_vu32_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_half register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUf16_vf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_float register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUf32_vf32_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_bfloat16_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUbf16_vbf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e4m3fn_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUf8e4m3_vf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_hifloat8_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUhf8_vhf8_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e5m2_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUf8e5m2_vf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e8m0_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUf8e8m0_vf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp4x2_e2m1_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUf4e2m1x2_vf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp4x2_e1m2_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUf4e1m2x2_vf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int4x2_t register to UB dst with an int32_t element offset; returns void.
 *
 * @override_id asc_storealign_1st__PUi4x2_vi4x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int16_t elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUi16_vi16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int16_t* dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint16_t elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUu16_vu16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int32_t elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint32_t elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUu32_vu32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int64_t elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUi64_vi64_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int64_t* dst, vector_int64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint64_t elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUu64_vu64_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint64_t* dst, vector_uint64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_half elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUf16_vf16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_float elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUf32_vf32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_bfloat16_t elements and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack__PUbf16_vbf16_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int8_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUi8_vi8_vi8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint8_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUu8_vu8_vu8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int16_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUi16_vi16_vi16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint16_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUu16_vu16_vu16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int32_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUi32_vi32_vi32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint32_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUu32_vu32_vu32_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_half registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUf16_vf16_vf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ half* dst, vector_half src0, vector_half src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_bfloat16_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUbf16_vbf16_vbf16_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e4m3fn_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUf8e4m3_vf8e4m3_vf8e4m3_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_hifloat8_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUhf8_vhf8_vhf8_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e5m2_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUf8e5m2_vf8e5m2_vf8e5m2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e8m0_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUf8e8m0_vf8e8m0_vf8e8m0_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp4x2_e2m1_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUf4e2m1x2_vf4e2m1x2_vf4e2m1x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp4x2_e1m2_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUf4e1m2x2_vf4e1m2x2_vf4e1m2x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int4x2_t registers and stores them to UB at the int32_t element offset from dst; the caller updates the offset or dst.
 *
 * @override_id asc_storealign_intlv__PUi4x2_vi4x2_vi4x2_i32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int4b_t* dst, vector_int4x2_t src0, vector_int4x2_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_int8_t data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUi8_vi8_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int8_t*& dst, vector_int8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_uint8_t data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUu8_vu8_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint8_t*& dst, vector_uint8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_int16_t data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUi16_vi16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_uint16_t data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUu16_vu16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_int32_t data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUi32_vi32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_uint32_t data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUu32_vu32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_half data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUf16_vf16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ half*& dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_float data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUf32_vf32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_bfloat16_t data (VL = 256 bytes) contiguously to the current UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUbf16_vbf16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址偏移`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_int8_t register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUi8_vi8_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int8_t*& dst, vector_int8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_uint8_t register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUu8_vu8_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint8_t*& dst, vector_uint8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_int16_t register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUi16_vi16_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_uint16_t register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUu16_vu16_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_int32_t register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUi32_vi32_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_uint32_t register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUu32_vu32_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_half register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUf16_vf16_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ half*& dst, vector_half src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_float register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUf32_vf32_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ float*& dst, vector_float src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the first element of the vector_bfloat16_t register to the dtype-aligned UB destination, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_1st_postupdate__RPUbf16_vbf16_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），搬运完成后该地址会被硬件自动Post Update。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int16_t elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUi16_vi16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint16_t elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUu16_vu16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int32_t elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUi32_vi32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint32_t elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUu32_vu32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int64_t elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUi64_vi64_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int64_t*& dst, vector_int64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint64_t elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUu64_vu64_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint64_t*& dst, vector_uint64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_half elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUf16_vf16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ half*& dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_float elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUf32_vf32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_bfloat16_t elements into UB, then automatically advances dst by offset elements (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUbf16_vbf16_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int8_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUi8_vi8_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint8_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUu8_vu8_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int16_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUi16_vi16_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint16_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUu16_vu16_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int32_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUi32_vi32_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_uint32_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUu32_vu32_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_half register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUf16_vf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_float register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUf32_vf32_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_bfloat16_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUbf16_vbf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e4m3fn_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUf8e4m3_vf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_hifloat8_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUhf8_vhf8_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e5m2_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUf8e5m2_vf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp8_e8m0_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUf8e8m0_vf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp4x2_e2m1_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUf4e2m1x2_vf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_fp4x2_e1m2_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUf4e1m2x2_vf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first element of a vector_int4x2_t register to UB dst with an addr_reg offset; uses asc_update_addr_reg; returns void.
 *
 * @override_id asc_storealign_1st__PUi4x2_vi4x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ <dtype>*`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int16_t elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUi16_vi16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint16_t elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUu16_vu16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int32_t elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUi32_vi32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint32_t elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUu32_vu32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_int64_t elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUi64_vi64_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int64_t* dst, vector_int64_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_uint64_t elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUu64_vu64_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint64_t* dst, vector_uint64_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_half elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUf16_vf16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_float elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUf32_vf32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low half bits of mask-selected vector_bfloat16_t elements and stores them to UB at the addr_reg offset from dst; requires an addr_reg built with asc_update_addr_reg.
 *
 * @override_id asc_storealign_pack__PUbf16_vbf16_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int8_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUi8_vi8_vi8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint8_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUu8_vu8_vu8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int16_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUi16_vi16_vi16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint16_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUu16_vu16_vu16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int32_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUi32_vi32_vi32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_uint32_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUu32_vu32_vu32_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_half registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUf16_vf16_vf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ half* dst, vector_half src0, vector_half src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_bfloat16_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUbf16_vbf16_vbf16_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e4m3fn_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUf8e4m3_vf8e4m3_vf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_hifloat8_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUhf8_vhf8_vhf8_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e5m2_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUf8e5m2_vf8e5m2_vf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp8_e8m0_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUf8e8m0_vf8e8m0_vf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp4x2_e2m1_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUf4e2m1x2_vf4e2m1x2_vf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_fp4x2_e1m2_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUf4e1m2x2_vf4e1m2x2_vf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Interleaves elements of two vector_int4x2_t registers and stores them to UB at the addr_reg offset from dst, for hardware loops using asc_update_addr_reg.
 *
 * @override_id asc_storealign_intlv__PUi4x2_vi4x2_vi4x2_ar
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_int8_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUi8_vi8_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int8_t*& dst, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_uint8_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUu8_vu8_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint8_t*& dst, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_int16_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUi16_vi16_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_uint16_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUu16_vu16_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_int32_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUi32_vi32_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_uint32_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUu32_vu32_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_bfloat16_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUbf16_vbf16_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_fp8_e4m3fn_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUf8e4m3_vf8e4m3_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_hifloat8_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUhf8_vhf8_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_fp8_e5m2_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUf8e5m2_vf8e5m2_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_fp8_e8m0_t data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUf8e8m0_vf8e8m0_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_half data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUf16_vf16_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ half*& dst, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores mask-selected vector_float data as 8 non-contiguous 32-byte DataBlocks with block_stride, then advances dst by repeat_stride (Post Update, 32-byte units).
 *
 * @override_id asc_storealign_postupdate__RPUf32_vf32_u16_u16_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。dtype须与dst一致。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits to UB at the addr_reg offset from dst, for hardware loops.
 *
 * @override_id asc_storealign_pack__PUu32_vb_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits to UB at the addr_reg offset from dst, for hardware loops.
 *
 * @override_id asc_storealign_pack__PUu16_vb_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits to UB at the addr_reg offset from dst, for hardware loops.
 *
 * @override_id asc_storealign_pack__PUu8_vb_ar
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits contiguously to the 16-byte-aligned UB address dst.
 *
 * @override_id asc_storealign_pack__PUu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits contiguously to the 16-byte-aligned UB address dst.
 *
 * @override_id asc_storealign_pack__PUu16_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits contiguously to the 16-byte-aligned UB address dst.
 *
 * @override_id asc_storealign_pack__PUu8_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits to UB at the int32_t byte offset from dst.
 *
 * @override_id asc_storealign_pack__PUu32_vb_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits to UB at the int32_t byte offset from dst.
 *
 * @override_id asc_storealign_pack__PUu16_vb_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
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

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register by discarding every other bit and stores the retained bits to UB at the int32_t byte offset from dst.
 *
 * @override_id asc_storealign_pack__PUu8_vb_i32
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需16字节对齐。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the 32-byte vector_bool mask register contiguously to the current UB destination, then automatically advances dst by offset bytes (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUu32_vb_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为字节。接口执行后，dst指针的底层UB地址偏移offset字节。
 */
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register (discarding every other bit) into UB, then automatically advances dst by offset bytes (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUu32_vb_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需16字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为字节。接口执行后，`dst`指针的底层UB地址更新`offset`字节。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the 32-byte vector_bool mask register contiguously to the current UB destination, then automatically advances dst by offset bytes (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUu16_vb_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为字节。接口执行后，dst指针的底层UB地址偏移offset字节。
 */
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register (discarding every other bit) into UB, then automatically advances dst by offset bytes (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUu16_vb_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需16字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为字节。接口执行后，`dst`指针的底层UB地址更新`offset`字节。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the 32-byte vector_bool mask register contiguously to the current UB destination, then automatically advances dst by offset bytes (Post Update mode).
 *
 * @override_id asc_storealign_postupdate__RPUu8_vb_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`），起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为字节。接口执行后，dst指针的底层UB地址偏移offset字节。
 */
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the vector_bool mask register (discarding every other bit) into UB, then automatically advances dst by offset bytes (Post Update mode).
 *
 * @override_id asc_storealign_pack_postupdate__RPUu8_vb_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需16字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（掩码寄存器）。
 * @param [in] offset 地址偏移量，类型为`int32_t`，单位为字节。接口执行后，`dst`指针的底层UB地址更新`offset`字节。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_int32_t element and stores them contiguously to the 32-byte-aligned UB address dst.
 *
 * @override_id asc_storealign_pack_quarter__PUi32_vi32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_uint32_t element and stores them contiguously to the 32-byte-aligned UB address dst.
 *
 * @override_id asc_storealign_pack_quarter__PUu32_vu32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_float element and stores them contiguously to the 32-byte-aligned UB address dst.
 *
 * @override_id asc_storealign_pack_quarter__PUf32_vf32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ float* dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_int32_t element and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack_quarter__PUi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_uint32_t element and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack_quarter__PUu32_vu32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_float element and stores them to UB at the int32_t element offset from dst.
 *
 * @override_id asc_storealign_pack_quarter__PUf32_vf32_i32_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_int32_t element and stores them to UB at the addr_reg offset from dst, for hardware loops.
 *
 * @override_id asc_storealign_pack_quarter__PUi32_vi32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_uint32_t element and stores them to UB at the addr_reg offset from dst, for hardware loops.
 *
 * @override_id asc_storealign_pack_quarter__PUu32_vu32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Compresses the low 8 bits of each mask-selected 32-bit vector_float element and stores them to UB at the addr_reg offset from dst, for hardware loops.
 *
 * @override_id asc_storealign_pack_quarter__PUf32_vf32_ar_vb
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ <dtype>*`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] offset 相对`dst`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the low 8 bits of valid 32-bit int32_t elements from the vector register into UB under mask control, then automatically advances dst by offset elements (offset x sizeof(dtype) bytes).
 *
 * @override_id asc_storealign_pack_quarter_postupdate__RPUi32_vi32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the low 8 bits of valid 32-bit uint32_t elements from the vector register into UB under mask control, then automatically advances dst by offset elements (offset x sizeof(dtype) bytes).
 *
 * @override_id asc_storealign_pack_quarter_postupdate__RPUu32_vu32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the low 8 bits of valid 32-bit float elements from the vector register into UB under mask control, then automatically advances dst by offset elements (offset x sizeof(dtype) bytes).
 *
 * @override_id asc_storealign_pack_quarter_postupdate__RPUf32_vf32_i32_vb
 * @archs 3510
 * @param [in,out] dst 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Writes the valid data byte count from the AR special register after asc_squeeze_and_storeunalign to UB at dst; the actual destination must be 4-byte aligned.
 *
 * @override_id asc_storealign_squeeze_status__PUu32
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的基地址，类型为`__ubuf__ uint32_t*`。无`offset`参数时，实际写入地址为`dst`；带`offset`参数时，实际写入地址为`dst`加`offset`字节。
 */
__simd_callee__ inline void asc_storealign_squeeze_status(__ubuf__ uint32_t* dst);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Writes the valid data byte count from the AR special register after asc_squeeze_and_storeunalign to UB at dst plus offset bytes; the destination must be 4-byte aligned.
 *
 * @override_id asc_storealign_squeeze_status__PUu32_i32
 * @archs 3510
 * @param [out] dst 目的操作数在UB上的基地址，类型为`__ubuf__ uint32_t*`。无`offset`参数时，实际写入地址为`dst`；带`offset`参数时，实际写入地址为`dst`加`offset`字节。
 * @param [in] offset 仅带`offset`参数的重载使用。相对`dst`基地址的字节偏移量，类型为`int32_t`；无`offset`参数时，默认值为`0`。
 */
__simd_callee__ inline void asc_storealign_squeeze_status(__ubuf__ uint32_t* dst, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Writes the valid data byte count from the AR special register after asc_squeeze_and_storeunalign to UB, then automatically advances the destination pointer by offset bytes.
 *
 * @override_id asc_storealign_squeeze_status_postupdate__RPUu32_i32
 * @archs 3510
 * @param [in,out] dst 目的操作数在UB上的基地址，类型为`__ubuf__ uint32_t*&`。接口执行时将当前AR值写入该地址，执行后将指针增加`offset`字节。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为字节。
 */
__simd_callee__ inline void asc_storealign_squeeze_status_postupdate(__ubuf__ uint32_t*& dst, int32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_int32_t elements to UB without an offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUi32_vi32_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_uint32_t elements to UB without an offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUu32_vu32_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_float elements to UB without an offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUf32_vf32_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_int32_t elements to UB using an int32_t element offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUi32_vi32_i32_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 相对`dst_align32b`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_uint32_t elements to UB using an int32_t element offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUu32_vu32_i32_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 相对`dst_align32b`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_float elements to UB using an int32_t element offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUf32_vf32_i32_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 相对`dst_align32b`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_int32_t elements to UB with automatic dst update; use asc_storealign_pack_quarter_postupdate instead.
 *
 * @override_id asc_storealign_pack_postupdate_v2__RPUi32_vi32_i32_vb
 * @archs 3510
 * @param [in,out] dst_align32b 目的操作数的起始地址，按指针引用传入，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_uint32_t elements to UB with automatic dst update; use asc_storealign_pack_quarter_postupdate instead.
 *
 * @override_id asc_storealign_pack_postupdate_v2__RPUu32_vu32_i32_vb
 * @archs 3510
 * @param [in,out] dst_align32b 目的操作数的起始地址，按指针引用传入，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter_postupdate)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_float elements to UB with automatic dst update; use asc_storealign_pack_quarter_postupdate instead.
 *
 * @override_id asc_storealign_pack_postupdate_v2__RPUf32_vf32_i32_vb
 * @archs 3510
 * @param [in,out] dst_align32b 目的操作数的起始地址，按指针引用传入，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_int32_t elements to UB using an addr_reg offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUi32_vi32_ar_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 地址寄存器偏移，类型为`addr_reg`，由配套的地址寄存器构造接口预先构造。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, addr_reg offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_uint32_t elements to UB using an addr_reg offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUu32_vu32_ar_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 地址寄存器偏移，类型为`addr_reg`，由配套的地址寄存器构造接口预先构造。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, addr_reg offset, vector_bool mask);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_storealign_pack_quarter)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: compresses the low 8 bits of mask-selected 32-bit vector_float elements to UB using an addr_reg offset; use asc_storealign_pack_quarter instead.
 *
 * @override_id asc_storealign_pack_v2__PUf32_vf32_ar_vb
 * @archs 3510
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 地址寄存器偏移，类型为`addr_reg`，由配套的地址寄存器构造接口预先构造。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ float* dst_align32b, vector_float src, addr_reg offset, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREALIGN_H
#endif
