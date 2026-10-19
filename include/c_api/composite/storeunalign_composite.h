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
#error "composite/storeunalign_composite.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_STOREUNALIGN_COMPOSITE_H
#endif

#ifndef INCLUDE_C_API_COMPOSITE_STOREUNALIGN_COMPOSITE_H
#define INCLUDE_C_API_COMPOSITE_STOREUNALIGN_COMPOSITE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/storeunalign_composite_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole int8_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUi8_vi8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst, vector_int8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole uint8_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUu8_vu8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ uint8_t* dst, vector_uint8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole fp4x2_e2m1_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUf4e2m1x2_vf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole fp4x2_e1m2_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUf4e1m2x2_vf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole int4b_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUi4x2_vi4x2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ int4b_t* dst, vector_int4x2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole fp8_e8m0_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUf8e8m0_vf8e8m0
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole fp8_e5m2_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUf8e5m2_vf8e5m2
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole fp8_e4m3fn_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUf8e4m3_vf8e4m3
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole hifloat8_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUhf8_vhf8
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole int16_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUi16_vi16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ int16_t* dst, vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole uint16_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUu16_vu16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ uint16_t* dst, vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole half vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUf16_vf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ half* dst, vector_half src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole bfloat16_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUbf16_vbf16
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole int32_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUi32_vi32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ int32_t* dst, vector_int32_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole uint32_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUu32_vu32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ uint32_t* dst, vector_uint32_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole float vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUf32_vf32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ float* dst, vector_float src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores the whole int64_t vector register (one VL, default 256 bytes) to a dtype-aligned Unified Buffer destination address (continuous store mode); void return.
 *
 * @override_id asc_store__PUi64_vi64
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器。
 */
__simd_callee__ inline void asc_store(__ubuf__ int64_t* dst, vector_int64_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count int8_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUi8_vi8_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst, vector_int8_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count uint8_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUu8_vu8_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ uint8_t* dst, vector_uint8_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count int16_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUi16_vi16_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ int16_t* dst, vector_int16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count uint16_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUu16_vu16_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ uint16_t* dst, vector_uint16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count int32_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUi32_vi32_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ int32_t* dst, vector_int32_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count uint32_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUu32_vu32_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ uint32_t* dst, vector_uint32_t src, uint32_t count);

__simd_callee__ inline void asc_store(__ubuf__ half* dst, vector_half src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count float elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUf32_vf32_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ float* dst, vector_float src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count int64_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUi64_vi64_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ int64_t* dst, vector_int64_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count bfloat16_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUbf16_vbf16_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count fp8_e8m0_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUf8e8m0_vf8e8m0_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count fp8_e5m2_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUf8e5m2_vf8e5m2_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count fp8_e4m3fn_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUf8e4m3_vf8e4m3_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count hifloat8_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUhf8_vhf8_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count fp4x2_e2m1_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUf4e2m1x2_vf4e2m1x2_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count fp4x2_e1m2_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUf4e1m2x2_vf4e1m2x2_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @brief Stores only the first count int4b_t elements of the source vector register to a dtype-aligned Unified Buffer destination address (partial store mode); void return.
 *
 * @override_id asc_store__PUi4x2_vi4x2_u32
 * @archs 3510
 * @param [out] dst 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址需按照`dtype`对齐。
 * @param [in] src 源矢量数据寄存器，`dtype`须与`dst`一致。仅前`count`个元素参与搬出，其余元素被忽略。
 * @param [in] count 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数。
 */
__simd_callee__ inline void asc_store(__ubuf__ int4b_t* dst, vector_int4x2_t src, uint32_t count);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_STOREUNALIGN_COMPOSITE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_STOREUNALIGN_COMPOSITE_H
#endif
