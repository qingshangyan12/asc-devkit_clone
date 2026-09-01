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
#error "datamove_atomic.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_ATOMIC_DATAMOVE_ATOMIC_H
#define INCLUDE_C_API_ATOMIC_DATAMOVE_ATOMIC_H

#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/atomic_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/atomic_impl.h"
#endif

/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_add_float()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_add_float();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_add_int8()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_add_int8();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_add_int16()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_add_int16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_add_int32()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_add_int32();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_disable_dma_atomic()
 * @archs 3510 2201
 */
__aicore__ inline void asc_disable_dma_atomic();
[[deprecated("NOTICE: asc_set_atomic_none is deprecated. Please use asc_disable_dma_atomic instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @function asc_set_atomic_none()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_none();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_add_bfloat()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_add_bfloat();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_add_float16()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_add_float16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_max_bfloat()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_max_bfloat();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_max_float()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_max_float();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_max_float16()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_max_float16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_max_int8()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_max_int8();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_max_int16()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_max_int16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_max_int32()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_max_int32();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_min_bfloat()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_min_bfloat();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_min_float()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_min_float();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_min_float16()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_min_float16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_min_int8()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_min_int8();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_min_int16()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_min_int16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @function asc_set_atomic_min_int32()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_min_int32();

[[deprecated("NOTICE: asc_set_atomic_add_int is deprecated. Please use asc_set_atomic_add_int32 instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @function asc_set_atomic_add_int()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_add_int();

[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @function asc_set_atomic_max_int()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_max_int();

[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @function asc_set_atomic_min_int()
 * @archs 3510 2201
 */
__aicore__ inline void asc_set_atomic_min_int();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
