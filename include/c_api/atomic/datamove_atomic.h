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
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ATOMIC_DATAMOVE_ATOMIC_H
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
 * @override_id asc_set_atomic_add_float__z
 * @archs 3510 2201
 * @brief 原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。本接口对后续目的地址为GM的数据搬运指令开启原子累加，不同产品支持的数据搬运通路请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_set_atomic_add_float();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_add_int8__z
 * @archs 3510 2201
 * @brief 原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。本接口对后续目的地址为GM的数据搬运指令开启原子累加，不同产品支持的数据搬运通路请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_set_atomic_add_int8();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_add_int16__z
 * @archs 3510 2201
 * @brief 原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。本接口对后续目的地址为GM的数据搬运指令开启原子累加，不同产品支持的数据搬运通路请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_set_atomic_add_int16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_add_int32__z
 * @archs 3510 2201
 * @brief 原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。本接口对后续目的地址为GM的数据搬运指令开启原子累加，不同产品支持的数据搬运通路请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_set_atomic_add_int32();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_disable_dma_atomic__z
 * @archs 3510 2201
 * @brief 与[asc_set_atomic_add](asc_set_atomic_add.md)、[asc_set_atomic_max](asc_set_atomic_max.md)、[asc_set_atomic_min](asc_set_atomic_min.md)接口配合使用，用于在完成原子操作后关闭原子操作，避免影响后续搬运。
 */
__aicore__ inline void asc_disable_dma_atomic();
[[deprecated("NOTICE: asc_set_atomic_none is deprecated. Please use asc_disable_dma_atomic instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_set_atomic_none__z
 * @archs 3510 2201
 * @brief **该接口已废弃，请使用[asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md)替代。
 */
__aicore__ inline void asc_set_atomic_none();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_add_bfloat__z
 * @archs 3510 2201
 * @brief 原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。本接口对后续目的地址为GM的数据搬运指令开启原子累加，不同产品支持的数据搬运通路请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_set_atomic_add_bfloat();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_add_float16__z
 * @archs 3510 2201
 * @brief 原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。本接口对后续目的地址为GM的数据搬运指令开启原子累加，不同产品支持的数据搬运通路请参考[约束说明](#约束说明)。
 */
__aicore__ inline void asc_set_atomic_add_float16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_max_bfloat__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取大操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取大后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最大值写入GM。
 */
__aicore__ inline void asc_set_atomic_max_bfloat();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_max_float__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取大操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取大后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最大值写入GM。
 */
__aicore__ inline void asc_set_atomic_max_float();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_max_float16__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取大操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取大后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最大值写入GM。
 */
__aicore__ inline void asc_set_atomic_max_float16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_max_int8__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取大操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取大后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最大值写入GM。
 */
__aicore__ inline void asc_set_atomic_max_int8();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_max_int16__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取大操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取大后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最大值写入GM。
 */
__aicore__ inline void asc_set_atomic_max_int16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_max_int32__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取大操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取大后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最大值写入GM。
 */
__aicore__ inline void asc_set_atomic_max_int32();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_min_bfloat__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取小操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取小后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最小值写入GM。
 */
__aicore__ inline void asc_set_atomic_min_bfloat();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_min_float__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取小操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取小后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最小值写入GM。
 */
__aicore__ inline void asc_set_atomic_min_float();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_min_float16__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取小操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取小后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最小值写入GM。
 */
__aicore__ inline void asc_set_atomic_min_float16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_min_int8__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取小操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取小后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最小值写入GM。
 */
__aicore__ inline void asc_set_atomic_min_int8();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_min_int16__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取小操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取小后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最小值写入GM。
 */
__aicore__ inline void asc_set_atomic_min_int16();
/**
 * @ingroup simd_c_api.atomic.datamove_atomic
 * @override_id asc_set_atomic_min_int32__z
 * @archs 3510 2201
 * @brief 设置对后续的从Unified Buffer（UB）/L0C Buffer/L1 Buffer到GM的数据传输开启原子比较取小操作。数据类型支持int8_t、int16_t、half、bfloat16_t、int32_t、float。 开启原子比较取小后，后续执行搬运操作从UB/L0C Buffer/L1 Buffer到GM时，将待拷贝的内容和GM已有内容进行比较，将最小值写入GM。
 */
__aicore__ inline void asc_set_atomic_min_int32();

[[deprecated("NOTICE: asc_set_atomic_add_int is deprecated. Please use asc_set_atomic_add_int32 instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_set_atomic_add_int__z
 * @archs 3510 2201
 * @brief **该接口已废弃，请使用[asc_set_atomic_add_int32](../atomic/datamove_atomic/asc_set_atomic_add.md)替代。
 */
__aicore__ inline void asc_set_atomic_add_int();

[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_set_atomic_max_int__z
 * @archs 3510 2201
 * @brief **该接口已废弃，请使用[asc_set_atomic_max_int32](../atomic/datamove_atomic/asc_set_atomic_max.md)替代。
 */
__aicore__ inline void asc_set_atomic_max_int();

[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_set_atomic_min_int__z
 * @archs 3510 2201
 * @brief **该接口已废弃，请使用[asc_set_atomic_min_int32](../atomic/datamove_atomic/asc_set_atomic_min.md)替代。
 */
__aicore__ inline void asc_set_atomic_min_int();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ATOMIC_DATAMOVE_ATOMIC_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ATOMIC_DATAMOVE_ATOMIC_H
#endif
