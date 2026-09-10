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
#error "composite/cube_datamove_composite.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_CUBE_DATAMOVE_COMPOSITE_H
#endif

#ifndef INCLUDE_C_API_COMPOSITE_CUBE_DATAMOVE_COMPOSITE_H
#define INCLUDE_C_API_COMPOSITE_CUBE_DATAMOVE_COMPOSITE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/cube_datamove_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/cube_datamove_impl.h"
#endif
/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12fb
 * @override_id asc_copy_l12fb_sync__PFBv_PL1v_u32
 * @archs 2201
 * @brief 将数据从L1 Buffer搬运到Fixpipe Buffer中，Fixpipe Buffer用于存放量化参数。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12bt
 * @override_id asc_copy_l12bt_sync__u64_PL1v_u32
 * @archs 2201
 * @brief 将数据从L1 Buffer搬运到BiasTable Buffer中，BiasTable Buffer用于存放矩阵计算中的Bias。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAf16_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAf16_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAf32_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAf32_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAi16_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAi16_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAi32_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAi32_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAu16_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAu16_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAu32_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAu32_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0a_sync__PCAbf16_bf16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0A Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l1_sync__PL1bf16_bf16_RCfillcfg
 * @archs 3510 2201
 * @brief 将一个标量填充值写入L1 Buffer的目的地址，并将填充值广播到多个32字节分形块中，可用于L1 Buffer的内存初始化。需要通过[asc_fill_value_config](../defs/union/asc_fill_value_config.md)配置的重复次数、单次分形块数、相邻重复之间的间隔。
 * @param [out] dst 目的操作数，目的矩阵的起始地址，存储位置为L1 Buffer，起始地址需要按照32字节对齐。
 * @param [in] value 源操作数，标量填充值。
 * @param [in] config 填充配置的结构体[asc_fill_value_config](../defs/union/asc_fill_value_config.md)，结构体说明见[表2](#fill_value_config_fields)。
 */
__aicore__ inline void asc_fill_l1_sync(
    __cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l1_sync__PL1bf16_f16_RCfillcfg
 * @archs 3510 2201
 * @brief 将一个标量填充值写入L1 Buffer的目的地址，并将填充值广播到多个32字节分形块中，可用于L1 Buffer的内存初始化。需要通过[asc_fill_value_config](../defs/union/asc_fill_value_config.md)配置的重复次数、单次分形块数、相邻重复之间的间隔。
 * @param [out] dst 目的操作数，目的矩阵的起始地址，存储位置为L1 Buffer，起始地址需要按照32字节对齐。
 * @param [in] value 源操作数，标量填充值。
 * @param [in] config 填充配置的结构体[asc_fill_value_config](../defs/union/asc_fill_value_config.md)，结构体说明见[表2](#fill_value_config_fields)。
 */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l1_sync__PL1bf16_u32_RCfillcfg
 * @archs 3510 2201
 * @brief 将一个标量填充值写入L1 Buffer的目的地址，并将填充值广播到多个32字节分形块中，可用于L1 Buffer的内存初始化。需要通过[asc_fill_value_config](../defs/union/asc_fill_value_config.md)配置的重复次数、单次分形块数、相邻重复之间的间隔。
 * @param [out] dst 目的操作数，目的矩阵的起始地址，存储位置为L1 Buffer，起始地址需要按照32字节对齐。
 * @param [in] value 源操作数，标量填充值。
 * @param [in] config 填充配置的结构体[asc_fill_value_config](../defs/union/asc_fill_value_config.md)，结构体说明见[表2](#fill_value_config_fields)。
 */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBf16_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBf16_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBf32_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBf32_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBi16_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBi16_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBi32_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBi32_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBu16_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBu16_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBu32_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBu32_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBbf16_bf16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBbf16_f16_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_fill_l0b_sync__PCBbf16_u32_RCfillcfg
 * @archs 2201
 * @brief 将value的二进制值赋值给dst，对L0B Buffer的Local Memory进行初始化。
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad1_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad2_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad2_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad3_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad3_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad4_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad4_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad5_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad5_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad6_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad6_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad7_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad7_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_pad8_sync__PL1v_PGv_u32
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad8_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1bf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1i32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1i8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1u32_PGu32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1u8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L1 Buffer中，并支持不同类型的pad模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1i8_PGi8_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1u8_PGu8_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1bf16_PGbf16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1f16_PGf16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1i16_PGi16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1f32_PGf32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1i32_PGi32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1_nd2nz
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1u32_PGu32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Level 1 cache (L1)，支持在数据搬运时进行ND格式到NZ格式的转换。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] nd_num 传输ND矩阵的数目，取值范围：nd_num∈[0, 4095]。
 * @param [in] n_value ND矩阵的行数，取值范围：n_value∈[0, 16384]。
 * @param [in] d_value ND矩阵的列数，取值范围：d_value∈[0, 65535]。
 * @param [in] src_nd_matrix_stride 源操作数相邻ND矩阵起始地址间的偏移，取值范围：src_nd_matrix_stride∈[0, 65535]，单位为元素。
 * @param [in] src_d_value 源操作数同一ND矩阵的相邻行起始地址间的偏移，取值范围：src_d_value∈[1, 65535]，单位为元素。
 * @param [in] dst_nz_c0_stride ND转换到NZ格式后，源操作数中的一行会转换为目的操作数的多行。dst_nz_c0_stride表示，目的NZ矩阵中，来自源操作数同一行的多行数据相邻起始地址间的偏移，取值范围：dst_nz_c0_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_n_stride 目的NZ矩阵中，Z型矩阵相邻行起始地址之间的偏移。取值范围：dst_nz_n_stride∈[1, 16384]，单位：C0_SIZE（32B）。
 * @param [in] dst_nz_matrix_stride 目的NZ矩阵中，相邻NZ矩阵起始地址间的偏移，取值范围：dst_nz_matrix_stride∈[1, 65535]，单位为元素。
 */
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t nd_num, uint16_t n_value, uint16_t d_value,
    uint16_t src_nd_matrix_stride, uint16_t src_d_value, uint16_t dst_nz_c0_stride, uint16_t dst_nz_n_stride,
    uint16_t dst_nz_matrix_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi4x2_PL1i4x2_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu8_PL1u8_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi8_PL1i8_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf16_PL1f16_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBbf16_PL1bf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu32_PL1u32_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi32_PL1i32_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf32_PL1f32_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf16_PL1f16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBbf16_PL1bf16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu32_PL1u32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi32_PL1i32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf32_PL1f32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi4x2_PL1i4x2_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu8_PL1u8_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi8_PL1i8_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf16_PL1f16_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAbf16_PL1bf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu32_PL1u32_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi32_PL1i32_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf32_PL1f32_u16_u8_u16_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi4x2_PL1i4x2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。包含2D格式搬运、3D格式搬运。
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] k_extension 该指令在目的操作数width维度的传输长度。如果不覆盖最右侧的分形，对于half类型，应为16的倍数，对于int8_t/uint8_t类型，应为32的倍数；如果覆盖最右侧的分形，则无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 该指令在目的操作数height维度的传输长度。如果不覆盖最下侧的分形，对于half/int8_t/uint8_t类型，应为16的倍数；如果覆盖最下侧的分形，则无倍数要求。取值范围：[1, 65535]。
 * @param [in] k_start_pt 该指令在目的操作数width维度的起点。对于half类型，应为16的倍数，对于int8_t/uint8_t类型，应为32的倍数。取值范围：[0, 65535]
 * @param [in] m_start_pt 该指令在目的操作数height维度的起点，如果不覆盖最下侧的分形，对于half/int8_t/uint8_t，应为16的倍数；如果覆盖最下侧的分形，则无倍数要求。取值范围：[0, 65535]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在filter_w的基础上将卷积核width增加256个元素。true表示增加；false表示不增加。
 * @param [in] filter_size_h 是否在filter_h的基础上将卷积核height增加256个元素。true表示增加；false表示不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置，仅在源操作数为half类型时有效。true表示启用；false表示不启用。
 * @param [in] f_matrix_ctrl 表示asc_copy_l12l0a指令从左矩阵还是右矩阵获取FeatureMap的属性描述，当前只支持设置为false。
 * @param [in] channel_size 源操作数的通道数，取值范围：[1, 63]。对于uint32_t/int32_t/float，channelSize可取值为4，N * 8，N * 8 + 4；对于half/bfloat16，channelSize可取值为4，8，N * 16，N * 16 + 4，N * 16 + 8；对于int8_t/uint8_t，channelSize可取值为4，8，16， 32 * N，N * 32 + 4，N * 32 + 8，N * 32 + 16；对于int4b_t，ChannelSize可取值为8，16，32，N * 64，N * 64 + 8，N * 64 + 16，N * 64 + 32。N为正整数。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu8_PL1u8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi8_PL1i8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf16_PL1f16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAbf16_PL1bf16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu32_PL1u32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi32_PL1i32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf32_PL1f32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0b_sparse_sync__PCBi8_PL1i8_PL1i8_u16_u8
 * @archs 2201
 * @brief 用于搬运存放在L1 Buffer里的512B大小的稠密权重矩阵到L0B Buffer里，同时读取128B大小的索引矩阵用于稠密矩阵的稀疏化。
 * @param [out] dst 目的操作数在L0B Buffer的起始地址。
 * @param [in] src 源操作数在L1 Buffer的起始地址。
 * @param [in] index 索引矩阵在L1 Buffer的起始地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位：512B。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 */
__aicore__ inline void asc_copy_l12l0b_sparse_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_trans_sync__PCAf16_PL1f16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0A Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_trans_sync__PCAbf16_PL1bf16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0A Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_trans_sync__PCAf32_PL1f32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0A Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_trans_sync__PCAi32_PL1i32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0A Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_trans_sync__PCAi8_PL1i8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0A Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_trans_sync__PCAu32_PL1u32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0A Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_trans_sync__PCAu8_PL1u8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0A Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBbf16_PL1bf16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf16_PL1f16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf32_PL1f32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBi32_PL1i32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBi8_PL1i8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBu32_PL1u32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBu8_PL1u8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。 对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据； 对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据； 对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。 对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B； 对于half/bfloat16_t数据类型，单位是16 × 16 × 2B； 对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B； 对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。false: 递增，每次迭代在前一个地址的基础上加上src_stride。true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGbf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGu8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGf32_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi32_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。- 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 - 不开启NZ2ND功能：取值范围为[1, 65535]。 - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：0：保留值；2：开启unit_flag，硬件执行完指令之后，不会设置寄存器；3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_load_image_to_cbuf_sync__PL1f16_u16_u16_u16_u16_u16_u8_u8_u16_u16
 * @archs 2201
 * @brief 将图像数据从Global Memory搬运到L1 Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] hor_size 从源图中加载图片的水平宽度，单位为像素。取值范围：[1, 4094]。
 * @param [in] ver_size 从源图中加载图片的垂直高度，单位为像素。取值范围：[1, 4094]。
 * @param [in] hor_start_pos 加载图片在源图片上的水平起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] ver_start_pos 加载图片在源图片上的垂直起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] src_hor_size 源图片水平宽度，单位为像素。取值范围：[1, 4094]。
 * @param [in] top_pad_size 目的图片顶部填充的像素数。取值范围：[0, 32]。
 * @param [in] bot_pad_size 目的图片底部填充的像素数。取值范围：[0, 32]。
 * @param [in] left_pad_size 目的图片左边填充的像素数。取值范围：[0, 32]。
 * @param [in] right_pad_size 目的图片右边填充的像素数。取值范围：[0, 32]。
 */
__aicore__ inline void asc_load_image_to_cbuf_sync(
    __cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_load_image_to_cbuf_sync__PL1i8_u16_u16_u16_u16_u16_u8_u8_u16_u16
 * @archs 2201
 * @brief 将图像数据从Global Memory搬运到L1 Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] hor_size 从源图中加载图片的水平宽度，单位为像素。取值范围：[1, 4094]。
 * @param [in] ver_size 从源图中加载图片的垂直高度，单位为像素。取值范围：[1, 4094]。
 * @param [in] hor_start_pos 加载图片在源图片上的水平起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] ver_start_pos 加载图片在源图片上的垂直起始地址，单位为像素。取值范围：[0, 4095]。
 * @param [in] src_hor_size 源图片水平宽度，单位为像素。取值范围：[1, 4094]。
 * @param [in] top_pad_size 目的图片顶部填充的像素数。取值范围：[0, 32]。
 * @param [in] bot_pad_size 目的图片底部填充的像素数。取值范围：[0, 32]。
 * @param [in] left_pad_size 目的图片左边填充的像素数。取值范围：[0, 32]。
 * @param [in] right_pad_size 目的图片右边填充的像素数。取值范围：[0, 32]。
 */
__aicore__ inline void asc_load_image_to_cbuf_sync(
    __cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size, uint16_t hor_start_pos, uint16_t ver_start_pos,
    uint16_t src_hor_size, uint8_t top_pad_size, uint8_t bot_pad_size, uint16_t left_pad_size, uint16_t right_pad_size);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBbf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBf16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBf32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBi32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBi8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBu32_PGu32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBu8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0b_sync__PCBi4x2_PGi4x2_u16_u8_u16_u16
 * @archs 2201
 * @brief 将矩阵数据从Global Memory搬运到L0B Buffer。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAbf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAf16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAf32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAi32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAu8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAi8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAu32_PGu32_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l0a_sync__PCAi4x2_PGi4x2_u16_u8_u16_u16
 * @archs 2201
 * @brief 将数据从GM搬运到L0A Buffer。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1f16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1bf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1i8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1f16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1i16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1i8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1u8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1v_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1v_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @brief 矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到L1 Buffer中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 4095]；&bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下：&bull; 0保留值；&bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器；&bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。&bull; false：不开启；&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0a_mx_sync__u64_PL1f8e8m0_u16_u16_u8_u8_u16_u16
 * @archs 3510
 * @brief 负责完成MX矩阵计算过程中所需的左矩阵对应的量化系数的搬运，数据通路为L1 Buffer->L0A_MX Buffer。其中左量化系数矩阵以32字节（固定数据类型为`fp8_e8m0_t`、分形大小为16×2的）的数据分形为单位进行搬运。
 * @param [out] dst 目的操作数，存储位置为L0A_MX Buffer。起始地址需要按照32字节对齐。取值范围：$[0, 2^{64}-1]$。
 * @param [in] src 源操作数，存储位置为L1 Buffer，数据类型固定为`fp8_e8m0_t`。起始地址需要按照32字节对齐。
 * @param [in] x_start_pos 源矩阵X轴起始分形位置，单位为32字节，即1个分形。取值范围：$[0, 2^{16}-1]$。
 * @param [in] y_start_pos 源矩阵Y轴起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] x_step 源矩阵X轴方向搬运长度，单位为1个分形，即32字节。取值范围：$[0, 255]$。
 * @param [in] y_step 源矩阵Y轴方向搬运长度，单位为32字节。取值范围：$[0, 255]$。
 * @param [in] src_stride 源矩阵X轴方向相邻分形起始地址的间隔，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] dst_stride 目的矩阵X轴方向相邻分形起始地址的间隔，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0b_mx_sync__u64_PL1f8e8m0_u16_u16_u8_u8_u16_u16
 * @archs 3510
 * @brief 负责完成MX矩阵计算过程中所需的右矩阵对应的量化系数的搬运，数据通路为L1 Buffer->L0B_MX Buffer。其中右量化系数矩阵以32字节（固定数据类型为`fp8_e8m0_t`、分形大小为2×16的）的数据分形为单位进行搬运。
 * @param [out] dst 目的操作数，存储位置为L0B_MX Buffer。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] x_start_pos 源矩阵X轴起始分形位置。单位为32字节，即1个2×16分形。
 * @param [in] y_start_pos 源矩阵Y轴起始分形位置。单位为32字节，对应分形的基本寻址粒度。
 * @param [in] x_step 源矩阵X轴方向搬运长度。单位为32字节，即1个2×16分形。取值范围：[1, 255]。
 * @param [in] y_step 源矩阵Y轴方向搬运长度。单位为32字节，对应分形的基本寻址粒度。取值范围：[1, 255]。
 * @param [in] src_stride 源矩阵X轴方向相邻分形起始地址的间隔。单位为32字节，即1个2×16分形。
 * @param [in] dst_stride 目的矩阵X轴方向相邻分形起始地址的间隔。单位为32字节，即1个2×16分形。
 */
__aicore__ inline void asc_copy_l12l0b_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0A Buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0A Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置，单位为16个元素。取值范围：$[0, 2^{16}-1]$。
 * @param [in] k_start_position 源矩阵列方向起始分形位置，单位为32字节。取值范围：$[0, 2^{16}-1]$。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位为32字节。取值范围：$[0, 255]$。转置场景下（调用`asc_copy_l12l0a_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[-2^{15}, 2^{15}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512字节。取值范围：$[1, 2^{16}-1]$。
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 * @brief 负责完成矩阵计算所需的2D格式数据的搬运，搬运粒度为大小为512字节的数据分形，支持非转置与转置矩阵从L1 Buffer到L0B buffer的数据搬运。
 * @param [out] dst 目的矩阵，存储位置为L0B Buffer。起始地址需要按照512字节对齐。
 * @param [in] src 源矩阵，存储位置为L1 Buffer。起始地址需要按照32字节对齐。
 * @param [in] m_start_position 源矩阵行方向起始分形位置。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, $2^{16}-1$]。
 * @param [in] k_start_position 源矩阵列方向起始分形位置。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, $2^{16}-1$]。
 * @param [in] m_step 源矩阵行方向搬运的分形行数。单位为16个元素，对应分形行方向的固定宽度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），m_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4时，m_step必须是4的倍数；&nbsp;&nbsp;&bull;当数据类型为b8时，m_step必须是2的倍数；&nbsp;&nbsp;&bull;当数据类型为b16时，m_step必须是1的倍数；&nbsp;&nbsp;&bull;当数据类型为b32时，m_step无额外约束。**注：m_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] k_step 源矩阵列方向搬运的分形列数。单位为32字节，对应分形列方向的基本寻址粒度。取值范围：[0, 255]。转置场景下（调用`asc_copy_l12l0b_transpose`），k_step除需满足取值范围外，还需满足以下额外约束：&nbsp;&nbsp;&bull;当数据类型为b4、b8或b16时，k_step没有额外约束；&nbsp;&nbsp;&bull;当数据类型为b32时，k_step必须是2的倍数。**注：k_step=0表示不执行搬运，该接口将被视为NOP（空操作）。**
 * @param [in] src_stride 源矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[$-2^{15}$, $2^{15}-1$]。
 * @param [in] dst_stride 目的矩阵列方向相邻分形起始地址的间隔。单位为512字节，即一个数据分形的大小。取值范围：[1, $2^{16}-1$]。
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBbf16_PL1bf16_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf8e4m3_PL1f8e4m3_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf8e5m2_PL1f8e5m2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf16_PL1f16_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf32_PL1f32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBhf8_PL1hf8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBi32_PL1i32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBi8_PL1i8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBu32_PL1u32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBu8_PL1u8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf4e1m2x2_PL1f4e1m2x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBf4e2m1x2_PL1f4e2m1x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b_trans
 * @override_id asc_copy_l12l0b_trans_sync__PCBi4x2_PL1i4x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @brief 该接口实现带转置的2D格式数据从L1 Buffer到L0B Buffer的加载。
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。 对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据； 对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据；对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12gm_sync__PGv_PL1v_u16_u16_u16_u16
 * @archs 2201
 * @brief 将数据从L1 Buffer搬运到Global Memory。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位为32字节。
 */
__aicore__ inline void asc_copy_l12gm_sync(
    __gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0c_sync__PCCbf16_PL1bf16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将矩阵由L1 Buffer搬运到L0C Buffer中。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0c_sync__PCCf16_PL1f16_u16_u16_u16_u16
 * @archs 2201
 * @brief 将矩阵由L1 Buffer搬运到L0C Buffer中。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0c_sync__PCCf16_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @brief 将矩阵由L1 Buffer搬运到L0C Buffer中。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0c_sync__PCCbf16_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @brief 将矩阵由L1 Buffer搬运到L0C Buffer中。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0c_sync__PCCf32_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @brief 将矩阵由L1 Buffer搬运到L0C Buffer中。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0c_sync__PCCi32_PL1i32_u16_u16_u16_u16
 * @archs 2201
 * @brief 将矩阵由L1 Buffer搬运到L0C Buffer中。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l12l0c_sync__PCCu32_PL1u32_u16_u16_u16_u16
 * @archs 2201
 * @brief 将矩阵由L1 Buffer搬运到L0C Buffer中。
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap,
    uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf8e4m3_PL1f8e4m3_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAf8e5m2_PL1f8e5m2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAhf8_PL1hf8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAi16_PL1i16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0a
 * @override_id asc_copy_l12l0a_sync__PCAu16_PL1u16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，然后再从展开后的二维矩阵中选取指定数据块搬入对应内存位置。见图1。
 * @param [out] dst 目的操作数，存储位置为L0A Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。&nbsp;&nbsp;&bull; `false`：不开启转置；&nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf8e4m3_PL1f8e4m3_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBf8e5m2_PL1f8e5m2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBhf8_PL1hf8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi8_PL1i8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu8_PL1u8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBi16_PL1i16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12l0b
 * @override_id asc_copy_l12l0b_sync__PCBu16_PL1u16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
 * @brief 3D卷积数据搬运是用于将NC1HWC0格式的Feature Map完成Image to Column展开，按卷积步长、卷积核大小、膨胀系数配置生成img2col二维矩阵，再从展开后的矩阵中选取指定数据块搬运到L0B Buffer，用于3D卷积场景的矩阵计算数据加载。
 * @param [out] dst 目的操作数，存储位置为L0B Buffer。目的地址需512字节对齐。
 * @param [in] src 源操作数，存储位置为L1 Buffer。源地址需32字节对齐。
 * @param [in] k_extension 目的操作数width维度的传输长度，如果不覆盖最右侧的分形，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；覆盖的情况则任何数据类型都无倍数要求。取值范围：[1, 65535]。
 * @param [in] m_extension 目的操作数height维度的传输长度，如果不覆盖最下侧的分形，对于b8、b16类型，应为16的倍数，b32类型无要求；覆盖的情况则任何数据类型都无倍数要求。取值范围：[0, 65535]。
 * @param [in] k_start_pt 目的操作数width维度的起点，对于b32类型，应为8的倍数，对于b16类型，应为16的倍数，对于b8类型，应为32的倍数；取值范围：[0, 65535]。
 * @param [in] m_start_pt 目的操作数height维度的起点，取值范围：[0, 32767]。
 * @param [in] stride_w 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。
 * @param [in] stride_h 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。
 * @param [in] filter_w 卷积核width，取值范围：[1, 255]。
 * @param [in] filter_h 卷积核height，取值范围：[1, 255]。
 * @param [in] dilation_filter_w 卷积核width膨胀系数，取值范围：[1, 255]。
 * @param [in] dilation_filter_h 卷积核height膨胀系数，取值范围：[1, 255]。
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。&nbsp;&nbsp;&bull; `true`：增加；&nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。&nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0_padding_val.md)、`asc_set_l13d_rpt_b`使用；&nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1bf16_PGbf16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1f16_PGf16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1f32_PGf32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1hf8_PGhf8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1i16_PGi16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1i32_PGi32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1i8_PGi8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1u16_PGu16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1u32_PGu32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_align_sync__PL1u8_PGu8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @brief 对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为字节。
 * @param [in] left_padding_count 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] right_padding_count 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。
 * @param [in] data_select_bit padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 * @param [in] burst_src_stride 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 * @param [in] burst_dst_stride 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1bf16_PGbf16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f8e4m3_PGf8e4m3_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f8e5m2_PGf8e5m2_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f16_PGf16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f32_PGf32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1hf8_PGhf8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1i16_PGi16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1i32_PGi32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1i8_PGi8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1u16_PGu16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1u32_PGu32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1u8_PGu8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，并在搬运过程中进行DN到Nz格式的转换，使其满足后续矩阵计算对分形列连续性的要求。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。&nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运；&nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1v_PGv_u32_u32_u8_u64_u32
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer，数据搬运时格式和内容保持不变。高维切分是指能够通过配置数据块个数、单个数据块长度、地址偏移等搬运参数实现非连续搬运。并且此接口支持在搬运过程中按通道padding模式对32字节粒度的数据插入或移除padding。其中padding值需要通过[asc_set_gm2l1_pad](../asc_set_gm2l1_pad.md)设置。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] n_burst 搬运的数据块的个数。取值范围：[1, $2^{17}-1$]。
 * @param [in] len_burst 搬运的每个数据块长度，单位32字节。取值范围：[1, $2^{17}-1$]。
 * @param [in] pad_func_mode 4 bit值，指示通道上的padding功能，在32字节数据中插入padding值，或从每个32字节的读取数据中移除padding值。取值范围 [0, 8]：&nbsp;&nbsp;&bull; `0`：不做padding；&nbsp;&nbsp;&bull; `1`：每有1个字节的数据，插入31个padding值；&nbsp;&nbsp;&bull; `2`：每有2个字节的数据，插入15个padding值；&nbsp;&nbsp;&bull; `3`：每有4个字节的数据，插入14个padding值；&nbsp;&nbsp;&bull; `4`：每有8个字节的数据，插入12个padding值；&nbsp;&nbsp;&bull; `5`：每有16个字节的数据，插入8个padding值；&nbsp;&nbsp;&bull; `6`：每有32个字节的数据，移除28个最高有效位数据，只保留4个最低有效位元素；&nbsp;&nbsp;&bull; `7`：每有32个字节的数据，移除24个最高有效位数据，只保留8个最低有效位元素；&nbsp;&nbsp;&bull; `8`：每有32个字节的数据，移除16个最高有效位数据，只保留16个最低有效位元素。
 * @param [in] src_stride 源操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{36}-1$]，单位32字节。&nbsp;&nbsp;&bull; n_burst = 1时，`src_stride`无意义，设置为0即可。
 * @param [in] dst_stride 目的操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{17}-1$]，单位32字节。&nbsp;&nbsp;&bull; n_burst = 1时，`dst_stride`无意义，设置为0即可。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12bt
 * @override_id asc_copy_l12bt_sync__u64_PL1bf16_u32
 * @archs 3510
 * @brief 将MMAD指令的Bias数据从L1 Buffer搬运到BiasTable Buffer中。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12bt
 * @override_id asc_copy_l12bt_sync__u64_PL1f16_u32
 * @archs 3510
 * @brief 将MMAD指令的Bias数据从L1 Buffer搬运到BiasTable Buffer中。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12bt
 * @override_id asc_copy_l12bt_sync__u64_PL1f32_u32
 * @archs 3510
 * @brief 将MMAD指令的Bias数据从L1 Buffer搬运到BiasTable Buffer中。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float* src, uint32_t size);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l12bt
 * @override_id asc_copy_l12bt_sync__u64_PL1i32_u32
 * @archs 3510
 * @brief 将MMAD指令的Bias数据从L1 Buffer搬运到BiasTable Buffer中。
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t* src, uint32_t size);

__aicore__ inline void asc_copy_l12ub_sync(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst,
    uint16_t src_gap, uint16_t dst_gap);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1i4x2_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 该接口用于将矩阵计算的结果从L0C Buffer搬运至L1 Buffer中，并且在搬运过程中支持与其它接口组合使用，配置多种随路能力。
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] l2_cache_mode 无效参数，用户无需关注，传入0即可。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; 0：不开启Clip ReLU；&nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; 0：不开启unit_flag；&nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位；&nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; 0：不开启ReLU；&nbsp;&nbsp;&bull; 1：开启Normal ReLU；&nbsp;&nbsp;&bull; 2：开启Scalar ReLU；&nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。&nbsp;&nbsp;&bull; false：不开启；&nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。&nbsp;&nbsp;&bull; false：不开启；&nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 * @param [in] broadcast_en 无效参数，用户无需关注，传入false即可。
 * @param [in] enable_nz2dn 是否开启NZ2DN功能。&nbsp;&nbsp;&bull; false：不开启；&nbsp;&nbsp;&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2l1
 * @override_id asc_copy_l0c2l1_sync__PL1i4x2_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 该接口用于将矩阵计算的结果从L0C Buffer搬运至L1 Buffer中，并且在搬运过程中支持与其它接口组合使用，配置多种随路能力。
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。&nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；&nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。&nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] l2_cache_mode 无效参数，用户无需关注，传入0即可。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; 0：不开启Clip ReLU；&nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; 0：不开启unit_flag；&nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位；&nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; 0：不开启ReLU；&nbsp;&nbsp;&bull; 1：开启Normal ReLU；&nbsp;&nbsp;&bull; 2：开启Scalar ReLU；&nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。&nbsp;&nbsp;&bull; false：不开启；&nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。&nbsp;&nbsp;&bull; false：不开启；&nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 * @param [in] broadcast_en 无效参数，用户无需关注，传入false即可。
 * @param [in] enable_nz2dn 是否开启NZ2DN功能。&nbsp;&nbsp;&bull; false：不开启；&nbsp;&nbsp;&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGbf16_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGf8e4m3_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGhf8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGu8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGf32_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGbf16_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGu8_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi32_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi4x2_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_l0c2gm
 * @override_id asc_copy_l0c2gm_sync__PGi4x2_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Global Memory，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`n_size`必须为2的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; 若`dst_dtype`设置为`int4b_t`，`m_size`必须为2的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于`dst_dtype`设置为`int4b_t`的输出场景，`dst_stride`必须为2的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(T)$，其中$T$为`src`的数据类型。取值范围为$[0, 2^{16}-1]$。
 * @param [in] l2_cache_mode 配置输出GM数据在L2 Cache中的管理策略。取值需为已定义的策略，即0、1、2或4，其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_mode_values)。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUbf16_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUf16_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUf8e4m3_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUhf8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUi8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUu8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUf32_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUbf16_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUf16_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUi8_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUu8_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUi32_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUi4x2_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove
 * @override_id asc_copy_l0c2ub_sync__PUi4x2_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @brief 将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（Nz2ND/Nz2DN）等能力组合。
 * @param [out] dst 目的操作数，存储位置为UB。起始地址需要按照32字节对齐。
 * @param [in] src 源操作数，存储位置为L0C Buffer。起始地址需要按照64字节对齐。
 * @param [in] n_size 源Nz矩阵在N方向上的大小，取值范围为$[1, 2^{12}-1]$。对于Nz输出场景：&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启Channel Split功能，`n_size`必须为8的倍数。&nbsp;&nbsp;&bull; 若不开启Channel Split功能，`n_size`必须为16的倍数。对于ND输出场景：&nbsp;&nbsp;&bull; $n\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`n_size`必须为64的倍数。
 * @param [in] m_size 源Nz矩阵在M方向上的大小，取值范围为$[1, 2^{16}-1]$。对于DN输出场景：&nbsp;&nbsp;&bull; $m\_size\times\operatorname{sizeof}(dst\_dtype)$必须为32的倍数；若`dst_dtype`设置为`int4b_t`，`m_size`必须为64的倍数。
 * @param [in] dst_stride 目的矩阵步长，取值范围为$[1, 2^{32}-1]$，对应的字节长度需32字节对齐。&nbsp;&nbsp;&bull; 若不开启Nz2ND功能，`dst_stride`表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位为元素。&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，`dst_stride`表示目的ND/DN矩阵每一行中的元素个数，单位为元素。对于不同`dst_dtype`的对齐约束如下：`int4b_t`输出需为64的倍数，8位输出需为32的倍数，16位输出需为16的倍数，32位输出需为8的倍数。
 * @param [in] src_stride 源Nz矩阵中相邻Z排布的起始地址偏移，单位为64字节，即$16\times\operatorname{sizeof}(src\_dtype)$。取值范围为$[0, 2^{16}-1]$。
 * @param [in] dual_dst_ctrl 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M×N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1。参数取值说明如下：&nbsp;&nbsp;&bull; `0`：单目标模式，将整个矩阵写入通过`sub_blockid`参数配置的目标UB。&nbsp;&nbsp;&bull; `1`：双目标模式，按M维度拆分成形状为$M/2\times N$的两个矩阵，分别写入两个UB，M必须为2的倍数。&nbsp;&nbsp;&bull; `2`：双目标模式，按N维度拆分成形状为$M\times N/2$的两个矩阵，分别写入两个UB，N须为32的倍数。该参数仅支持普通搬运模式（Nz2Nz）或Nz2ND搬运场景，不支持其他随路功能场景。
 * @param [in] sub_blockid 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU。&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar模式）。
 * @param [in] unit_flag_mode UnitFlag是MMAD类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：&nbsp;&nbsp;&bull; `0`：不开启UnitFlag。&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位。&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。开启该功能时，须将MMAD类指令和矩阵搬出类指令的UnitFlag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化模式，取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下：&nbsp;&nbsp;&bull; `0`：不开启ReLU。&nbsp;&nbsp;&bull; `1`：开启Normal ReLU。&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU。&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分功能。仅在`src_dtype`和`dst_dtype`均为`float`且输出为Nz格式时可开启。&nbsp;&nbsp;&bull; `false`：不开启。&nbsp;&nbsp;&bull; `true`：开启。
 * @param [in] enable_nz2nd Nz2ND格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2ND转换。&nbsp;&nbsp;&bull; `true`：开启Nz2ND转换。
 * @param [in] quant_post 预留参数，当前须设置为0。
 * @param [in] relu_post 预留参数，当前须设置为0。
 * @param [in] clip_relu_post 预留参数，当前须设置为`false`。
 * @param [in] eltwise_op 预留参数，当前须设置为0。
 * @param [in] eltwise_antq_en 预留参数，当前须设置为`false`。
 * @param [in] c0_pad_en 预留参数，当前须设置为`false`。
 * @param [in] broadcast_en 预留参数，当前须设置为`false`。
 * @param [in] enable_nz2dn Nz2DN格式转换使能。&nbsp;&nbsp;&bull; `false`：关闭Nz2DN转换。&nbsp;&nbsp;&bull; `true`：开启Nz2DN转换。
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1bf16_PGbf16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f32_PGf32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f8e4m3_PGf8e4m3_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f8e5m2_PGf8e5m2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f16_PGf16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1hf8_PGhf8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1i16_PGi16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1i32_PGi32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1i8_PGi8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1u16_PGu16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1u32_PGu32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1u8_PGu8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f4e1m2x2_PGf4e1m2x2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1f4e2m1x2_PGf4e2m1x2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

/**
 * @ingroup simd_c_api.cube_datamove.asc_copy_gm2l1
 * @override_id asc_copy_gm2l1_sync__PL1v_PGv_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
 * @brief 将数据从Global Memory搬运到L1 Buffer。以512字节的分形为单位，从GM中按行列方向的起始位置、步长切分提取多个分形矩阵搬运到L1 Buffer，搬运过程中不支持分形格式转换，用于矩阵计算所需的2D格式数据加载。
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] m_start_position 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。
 * @param [in] k_start_position 源矩阵中首个分形在列方向的起始位置，单位32字节。取值范围：$[0, 2^{32}-1]$。
 * @param [in] dst_stride 目的矩阵列方向前一个分形起始地址与后一个分形起始地址的间隔，单位512字节。取值范围：[0, 4095]。
 * @param [in] m_step 源矩阵行方向分形步长，即搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。
 * @param [in] k_step 源矩阵列方向分形步长，即搬运的分形列数，单位32字节。取值范围：[1, 4095]。
 * @param [in] decomp_mode 预留参数，当前须设置为0。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_CUBE_DATAMOVE_COMPOSITE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_CUBE_DATAMOVE_COMPOSITE_H
#endif
