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
#error "scalar_load.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_SCALAR_LOAD_H
#endif

#ifndef INCLUDE_C_API_SCALAR_COMPUTE_SCALAR_LOAD_H
#define INCLUDE_C_API_SCALAR_COMPUTE_SCALAR_LOAD_H

#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/scalar_load_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a int8_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGi8
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline int8_t asc_load_dev(__gm__ int8_t* addr);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a uint8_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGu8
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline uint8_t asc_load_dev(__gm__ uint8_t* addr);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a int16_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGi16
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline int16_t asc_load_dev(__gm__ int16_t* addr);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a uint16_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGu16
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline uint16_t asc_load_dev(__gm__ uint16_t* addr);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a int32_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGi32
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline int32_t asc_load_dev(__gm__ int32_t* addr);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a uint32_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGu32
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline uint32_t asc_load_dev(__gm__ uint32_t* addr);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a int64_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGi64
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline int64_t asc_load_dev(__gm__ int64_t* addr);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_load
 * @brief Reads and returns a uint64_t value directly from the GM address addr bypassing DCache, for multi-core access not aligned to Cache Lines (non-void return, PIPE_S).
 *
 * @override_id asc_load_dev__PGu64
 * @archs 3510
 * @param [in] addr 源GM地址。支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`uint64_t`。
 */
__aicore__ inline uint64_t asc_load_dev(__gm__ uint64_t* addr);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_SCALAR_LOAD_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_SCALAR_LOAD_H
#endif
