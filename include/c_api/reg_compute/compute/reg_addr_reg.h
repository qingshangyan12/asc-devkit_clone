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
#error "compute/reg_addr_reg.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ADDR_REG_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_ADDR_REG_H
#define INCLUDE_C_API_REG_COMPUTE_COMPUTE_REG_ADDR_REG_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/reg_addr_reg_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b32 (4-byte unit) address register with 1 offset argument(s) in elements, for 1-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b32__u32
 * @archs 3510
 * @param [in] offset 第1维的偏移量（最内层），单位为元素个数。 数据类型为uint32_t。 例如b32模式下offset=64表示64个float元素（一个VL即256B）。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b16 (2-byte unit) address register with 1 offset argument(s) in elements, for 1-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b16__u32
 * @archs 3510
 * @param [in] offset 第1维的偏移量（最内层），单位为元素个数。 数据类型为uint32_t。 例如b32模式下offset=64表示64个float元素（一个VL即256B）。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b8 (1-byte unit) address register with 1 offset argument(s) in elements, for 1-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b8__u32
 * @archs 3510
 * @param [in] offset 第1维的偏移量（最内层），单位为元素个数。 数据类型为uint32_t。 例如b32模式下offset=64表示64个float元素（一个VL即256B）。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b32 (4-byte unit) address register with 2 offset argument(s) in elements, for 2-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b32__u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset0, uint32_t offset1);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b16 (2-byte unit) address register with 2 offset argument(s) in elements, for 2-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b16__u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset0, uint32_t offset1);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b8 (1-byte unit) address register with 2 offset argument(s) in elements, for 2-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b8__u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset0, uint32_t offset1);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b32 (4-byte unit) address register with 3 offset argument(s) in elements, for 3-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b32__u32_u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b16 (2-byte unit) address register with 3 offset argument(s) in elements, for 3-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b16__u32_u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b8 (1-byte unit) address register with 3 offset argument(s) in elements, for 3-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b8__u32_u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b32 (4-byte unit) address register with 4 offset argument(s) in elements, for 4-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b32__u32_u32_u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset3 第4维的偏移量（最外层，如N×C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b16 (2-byte unit) address register with 4 offset argument(s) in elements, for 4-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b16__u32_u32_u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset3 第4维的偏移量（最外层，如N×C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @brief Updates and returns an addr_reg b8 (1-byte unit) address register with 4 offset argument(s) in elements, for 4-D iterative addressing.
 *
 * @override_id asc_update_addr_reg_b8__u32_u32_u32_u32
 * @archs 3510
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 * @param [in] offset3 第4维的偏移量（最外层，如N×C×H×W维的总步长），单位为元素个数。 数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b32 (4-byte unit) address register with 1 offset argument(s) for 1-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b32__u32
 * @archs 3510
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(uint32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b16 (2-byte unit) address register with 1 offset argument(s) for 1-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b16__u32
 * @archs 3510
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(uint32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b8 (1-byte unit) address register with 1 offset argument(s) for 1-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b8__u32
 * @archs 3510
 * @param [in] offset 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(uint32_t offset);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b32 (4-byte unit) address register with 2 offset argument(s) for 2-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b32__u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b16 (2-byte unit) address register with 2 offset argument(s) for 2-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b16__u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b8 (1-byte unit) address register with 2 offset argument(s) for 2-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b8__u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b32 (4-byte unit) address register with 3 offset argument(s) for 3-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b32__u32_u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b16 (2-byte unit) address register with 3 offset argument(s) for 3-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b16__u32_u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b8 (1-byte unit) address register with 3 offset argument(s) for 3-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b8__u32_u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b32)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b32 (4-byte unit) address register with 4 offset argument(s) for 4-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b32__u32_u32_u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 * @param [in] offset3 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b32(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b16)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b16 (2-byte unit) address register with 4 offset argument(s) for 4-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b16__u32_u32_u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 * @param [in] offset3 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b16(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_update_addr_reg_b8)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: initializes and returns an iter_reg b8 (1-byte unit) address register with 4 offset argument(s) for 4-D iterative addressing; replaced by asc_update_addr_reg.
 *
 * @override_id asc_create_iter_reg_b8__u32_u32_u32_u32
 * @archs 3510
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 * @param [in] offset3 地址偏移量。
 */
__simd_callee__ inline addr_reg asc_create_iter_reg_b8(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ADDR_REG_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ADDR_REG_H
#endif
