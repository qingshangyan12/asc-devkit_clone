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
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b32__u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset 第1维的偏移量（最内层），单位为元素个数。数据类型为uint32_t。例如b32模式下offset=64表示64个float元素（一个VL即256B）。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b16__u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset 第1维的偏移量（最内层），单位为元素个数。数据类型为uint32_t。例如b32模式下offset=64表示64个float元素（一个VL即256B）。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b8__u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset 第1维的偏移量（最内层），单位为元素个数。数据类型为uint32_t。例如b32模式下offset=64表示64个float元素（一个VL即256B）。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b32__u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset0, uint32_t offset1);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b16__u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset0, uint32_t offset1);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b8__u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset0, uint32_t offset1);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b32__u32_u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b16__u32_u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b8__u32_u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b32__u32_u32_u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset3 第4维的偏移量（最外层，如N×C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b32(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b16__u32_u32_u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset3 第4维的偏移量（最外层，如N×C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b16(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_update_addr_reg_b8__u32_u32_u32_u32
 * @archs 3510
 * @brief 更新地址寄存器（[addr_reg](../../defs/type/data_type_definition.md#地址寄存器)），用于迭代寻址。
 * @param [in] offset0 第1维的偏移量（最内层，如W维），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset1 第2维的偏移量（如H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset2 第3维的偏移量（如C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 * @param [in] offset3 第4维的偏移量（最外层，如N×C×H×W维的总步长），单位为元素个数。数据类型为uint32_t。
 */
__simd_callee__ inline addr_reg asc_update_addr_reg_b8(
    uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b32__u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b32(uint32_t offset);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b16__u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b16(uint32_t offset);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b8__u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b8(uint32_t offset);

[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b32__u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b16__u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b8__u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1);

[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b32__u32_u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b16__u32_u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b8__u32_u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2);

[[deprecated("NOTICE: asc_create_iter_reg_b32 is deprecated. Please use asc_update_addr_reg_b32 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b32__u32_u32_u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 * @param [in] offset3 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b32(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

[[deprecated("NOTICE: asc_create_iter_reg_b16 is deprecated. Please use asc_update_addr_reg_b16 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b16__u32_u32_u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 * @param [in] offset3 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b16(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

[[deprecated("NOTICE: asc_create_iter_reg_b8 is deprecated. Please use asc_update_addr_reg_b8 "
/**
 * @ingroup simd_c_api.reg_compute.reg_addr_reg
 * @override_id asc_create_iter_reg_b8__u32_u32_u32_u32
 * @archs 3510
 * @brief **该接口已废弃，请使用[asc_update_addr_reg](asc_update_addr_reg.md)实现此功能。
 * @param [in] offset0 地址偏移量。
 * @param [in] offset1 地址偏移量。
 * @param [in] offset2 地址偏移量。
 * @param [in] offset3 地址偏移量。
 */
             "instead.")]] __simd_callee__ inline addr_reg
asc_create_iter_reg_b8(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ADDR_REG_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_COMPUTE_REG_ADDR_REG_H
#endif
