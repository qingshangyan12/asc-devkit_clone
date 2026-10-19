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
#include "impl/c_api/memory_base_impl/cube_datamove_composite_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/cube_datamove_composite_intf_impl.h"
#endif
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12fb and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy writing quantization parameters from L1 Buffer src into Fixpipe Buffer dst (void return, size in bytes); superseded by asc_copy_l12fb plus asc_sync.
 *
 * @override_id asc_copy_l12fb_sync__PFBv_PL1v_u32
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy writing Bias data from L1 Buffer src into BiasTable Buffer dst (void return, size in bytes); superseded by asc_copy_l12bt plus asc_sync.
 *
 * @override_id asc_copy_l12bt_sync__u64_PL1v_u32
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a half value into half dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAf16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a uint32_t scalar, converted to the dst dtype into half dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAf16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a half value, converted to the dst dtype into float dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAf32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a uint32_t scalar bit pattern (0x3F800000 becomes 1.0) into float dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAf32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a half value, converted to the dst dtype into int16_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAi16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a uint32_t scalar, converted to the dst dtype into int16_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAi16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a half value, converted to the dst dtype into int32_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAi32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a uint32_t scalar, converted to the dst dtype into int32_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAi32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a half value, converted to the dst dtype into uint16_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAu16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a uint32_t scalar, converted to the dst dtype into uint16_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAu16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a half value, converted to the dst dtype into uint32_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAu32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a uint32_t scalar, converted to the dst dtype into uint32_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAu32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous fill of L0A Buffer: writes the binary value of a bfloat16_t value into bfloat16_t dst; use asc_fill_l0a plus asc_sync instead. A2/A3 arch 2201.
 *
 * @override_id asc_fill_l0a_sync__PCAbf16_bf16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0a_sync(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a bfloat16_t scalar into L1 Buffer dst and broadcasts it to 32-byte fractal blocks per asc_fill_value_config (void return); syncs internally; prefer asc_fill_l1.
 *
 * @override_id asc_fill_l1_sync__PL1bf16_bf16_RCfillcfg
 * @archs 2201 3510
 */
__aicore__ inline void asc_fill_l1_sync(
    __cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half scalar into L1 Buffer dst and broadcasts it to 32-byte fractal blocks per asc_fill_value_config (void return); syncs internally; prefer asc_fill_l1.
 *
 * @override_id asc_fill_l1_sync__PL1bf16_f16_RCfillcfg
 * @archs 2201 3510
 */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t scalar into L1 Buffer dst and broadcasts it to 32-byte fractal blocks per asc_fill_value_config (void return); syncs internally; prefer asc_fill_l1.
 *
 * @override_id asc_fill_l1_sync__PL1bf16_u32_RCfillcfg
 * @archs 2201 3510
 */
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half value into every half element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBf16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t value into every half element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBf16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half value into every float element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBf32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t value into every float element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBf32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half value into every int16_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBi16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t value into every int16_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBi16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half value into every int32_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBi32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t value into every int32_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBi32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half value into every uint16_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBu16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t value into every uint16_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBu16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half value into every uint32_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBu32_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t value into every uint32_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBu32_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a bfloat16_t value into every bfloat16_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBbf16_bf16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a half value into every bfloat16_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBbf16_f16_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, half value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fill_l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fill: writes a uint32_t value into every bfloat16_t element of dst to initialize L0B Buffer local memory; use asc_fill_l0b and asc_sync instead (void; result in dst).
 *
 * @override_id asc_fill_l0b_sync__PCBbf16_u32_RCfillcfg
 * @archs 2201
 * @param [out] dst 目的操作数（矩阵）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] config 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../defs/union/asc_fill_value_config.md)。
 */
__aicore__ inline void asc_fill_l0b_sync(__cb__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst without padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad1 layout (31B pad per 1B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad1_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad2 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad2 layout (15B pad per 1B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad2_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad2_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad3 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad3 layout (14B pad per 2B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad3_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad3_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad4 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad4 layout (12B pad per 4B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad4_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad4_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad5 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad5 layout (8B pad per 8B in each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad5_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad5_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad6 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad6 layout (keep only lowest 4B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad6_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad6_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad7 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad7 layout (keep only lowest 8B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad7_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad7_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_pad8 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): copies size bytes from GM src to L1 Buffer dst with pad8 layout (keep only lowest 16B of each 32B); result written to dst.
 *
 * @override_id asc_copy_gm2l1_pad8_sync__PL1v_PGv_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2l1_pad8_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): 2D copies bfloat16_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations with src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1bf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): 2D copies half matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations with src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): 2D copies float matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations with src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): 2D copies int32_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations with src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1i32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): 2D copies int8_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations with src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1i8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): 2D copies uint32_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations with src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1u32_PGu32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync form (arch 2201): 2D copies uint8_t matrix data from GM src to L1 Buffer dst at fractal base_idx for repeat iterations with src_stride/dst_gap; result written to dst.
 *
 * @override_id asc_copy_gm2l1_sync__PL1u8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 分形矩阵ID，说明搬运起始位置为源操作数中的第几个分形（0为源操作数中的第一个分形矩阵）。取值范围：[0, 65535]。单位512B。默认为0。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。默认为0。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65536]。默认为0。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int8_t data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1i8_PGi8_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint8_t data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1u8_PGu8_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of bfloat16_t data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1bf16_PGbf16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of half data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1f16_PGf16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int16_t data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1i16_PGi16_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of float data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1f32_PGf32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int32_t data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1i32_PGi32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint32_t data from Global Memory to L1 cache with ND-to-NZ layout conversion; replaced by asc_copy_gm2l1_nd2nz_arch_2201 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_nd2nz_sync__PL1u32_PGu32_u16_u16_u16_u16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int4b_t 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi4x2_PL1i4x2_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint8_t 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu8_PL1u8_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int8_t 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi8_PL1i8_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of half 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf16_PL1f16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of bfloat16_t 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBbf16_PL1bf16_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint32_t 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu32_PL1u32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int32_t 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi32_PL1i32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of float 512B matrices from L1 Buffer to L0B Buffer supporting 2D and 3D (img2col) formats; replaced by asc_copy_l12l0b_arch_2201 with asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf32_PL1f32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a half NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf16_PL1f16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a bfloat16_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBbf16_PL1bf16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a uint32_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu32_PL1u32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a int32_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi32_PL1i32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a float NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf32_PL1f32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer int4b_t source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi4x2_PL1i4x2_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer uint8_t source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu8_PL1u8_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer int8_t source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi8_PL1i8_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer half source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf16_PL1f16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的L0A Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位为512字节。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分型起始地址的间隔。取值范围：[0, 65535]。单位为512字节。
 * @param [in] dst_gap 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。取值范围：[0, 65535]。单位为512字节。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer bfloat16_t source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAbf16_PL1bf16_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer uint32_t source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu32_PL1u32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer int32_t source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi32_PL1i32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies 512-byte fractal matrices from an L1 Buffer float source to an L0A Buffer in 2D format; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf32_PL1f32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that copies L1 Buffer int4b_t data to an L0A Buffer in 3D img2col format with filter, stride and transpose options; use asc_copy_l12l0a with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi4x2_PL1i4x2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a uint8_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu8_PL1u8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a int8_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi8_PL1i8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a half NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf16_PL1f16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a bfloat16_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAbf16_PL1bf16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a uint32_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu32_PL1u32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a int32_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi32_PL1i32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a float NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf32_PL1f32_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 2201 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_sparse and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of a dense 512B weight matrix per repeat from L1 Buffer src to L0B Buffer dst with a 128B L1 index matrix for sparsification; use asc_copy_l12l0b_sparse plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sparse_sync__PCBi8_PL1i8_PL1i8_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数在L0B Buffer的起始地址。
 * @param [in] src 源操作数在L1 Buffer的起始地址。
 * @param [in] index 索引矩阵在L1 Buffer的起始地址。
 * @param [in] start_index 分形矩阵ID，说明搬运起始位置为源操作数中第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。单位：512B。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 */
__aicore__ inline void asc_copy_l12l0b_sparse_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of half data from L1 Buffer to L0A Buffer; use asc_copy_l12l0a_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_trans_sync__PCAf16_PL1f16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of bfloat16_t data from L1 Buffer to L0A Buffer; use asc_copy_l12l0a_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_trans_sync__PCAbf16_PL1bf16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of float data from L1 Buffer to L0A Buffer; use asc_copy_l12l0a_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_trans_sync__PCAf32_PL1f32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of int32_t data from L1 Buffer to L0A Buffer; use asc_copy_l12l0a_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_trans_sync__PCAi32_PL1i32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of int8_t data from L1 Buffer to L0A Buffer; use asc_copy_l12l0a_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_trans_sync__PCAi8_PL1i8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of uint32_t data from L1 Buffer to L0A Buffer; use asc_copy_l12l0a_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_trans_sync__PCAu32_PL1u32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of uint8_t data from L1 Buffer to L0A Buffer; use asc_copy_l12l0a_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0a_trans_sync__PCAu8_PL1u8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0a_trans_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of bfloat16_t data from L1 Buffer to L0B Buffer (arch 2201); use asc_copy_l12l0b_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBbf16_PL1bf16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of half data from L1 Buffer to L0B Buffer (arch 2201); use asc_copy_l12l0b_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf16_PL1f16_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of float data from L1 Buffer to L0B Buffer (arch 2201); use asc_copy_l12l0b_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf32_PL1f32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of int32_t data from L1 Buffer to L0B Buffer (arch 2201); use asc_copy_l12l0b_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBi32_PL1i32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of int8_t data from L1 Buffer to L0B Buffer (arch 2201); use asc_copy_l12l0b_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBi8_PL1i8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of uint32_t data from L1 Buffer to L0B Buffer (arch 2201); use asc_copy_l12l0b_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBu32_PL1u32_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous transposed 2D-format load of uint8_t data from L1 Buffer to L0B Buffer (arch 2201); use asc_copy_l12l0b_trans with asc_sync instead.
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBu8_PL1u8_u16_u8_u16_u16_b_u16
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数。
 * @param [in] index_id 方块矩阵的ID，搬运起始位置为源操作数中第几个方块矩阵（0为源操作数中第1个方块矩阵）。取值范围：index_id∈[0, 65535]。
 * @param [in] repeat 迭代重复次数。repeat∈[0, 255]。  对于uint8_t/int8_t数据类型，每次迭代处理32 × 32 × 1B数据；  对于half/bfloat16_t数据类型，每次迭代处理16 × 16 × 2B数据；  对于float/int32_t/uint32_t数据类型，每次迭代处理16 × 16 × 4B数据。
 * @param [in] src_stride 表示相邻迭代间，源操作数前一个方块矩阵与后一个方块矩阵起始地址的间隔，单位：（1024B）。取值范围：src_stride∈[0, 65535]。  对于uint8_t/int8_t数据类型，单位是32 × 32 × 1B；   对于half/bfloat16_t数据类型，单位是16 × 16 × 2B；  对于float/int32_t/uint32_t数据类型，单位是16 × 16 × 4B；   对于int4b_t数据类型，每次迭代处理16 × 64 × 0.5B数据。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。取值范围：dst_gap∈[0, 65535]。
 * @param [in] enable_addr_decrement 控制地址更新方式。 false: 递增，每次迭代在前一个地址的基础上加上src_stride。 true: 递减，每次迭代在前一个地址的基础上减去src_stride。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔，单位为512B，仅在数据类型为float/int32_t/uint32_t/uint8_t/int8_t时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, bool enable_addr_decrement, uint16_t dst_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to a GM half destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to a GM bfloat16_t destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGbf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to a GM int8_t destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to a GM uint8_t destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGu8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to a GM float destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGf32_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to a GM half destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to a GM int16_t destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to a GM int8_t destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to a GM int32_t destination; use asc_copy_l0c2gm with asc_sync instead.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi32_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。  - 不开启NZ2ND功能：若开启channel_split功能，n_size必须为8的倍数，取值范围为[1, 4095]。若不开启channel_split功能，n_size必须为16的倍数，取值范围为[1, 4095]。 - 开启NZ2ND功能：m_size的取值范围为[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。  - 不开启NZ2ND功能：取值范围为[1, 65535]。  - 开启NZ2ND功能，m_size的取值范围为[1, 8192]。
 * @param [in] dst_stride_dst_d - 不开启NZ2ND功能,目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。  - 开启NZ2ND/NZ2DN功能,目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： 0：保留值； 2：开启unit_flag，硬件执行完指令之后，不会设置寄存器； 3：开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] nz2nd_en 开启nz2nd开关，false：不开启；true：开启。
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride_dst_d,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool channel_split,
    bool nz2nd_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_image_to_cbuf and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous overload loads an image from Global Memory into L1 Buffer dst (half) with size, start positions, src_hor_size and four-side padding; use asc_load_image_to_cbuf plus asc_sync.
 *
 * @override_id asc_load_image_to_cbuf_sync__PL1f16_u16_u16_u16_u16_u16_u8_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_image_to_cbuf and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous overload loads an image from Global Memory into L1 Buffer dst (int8_t) with size, start positions, src_hor_size and four-side padding; use asc_load_image_to_cbuf plus asc_sync.
 *
 * @override_id asc_load_image_to_cbuf_sync__PL1i8_u16_u16_u16_u16_u16_u8_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of bfloat16_t matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBbf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of half matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBf16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of float matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBf32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int32_t matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBi32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int8_t matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBi8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint32_t matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBu32_PGu32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint8_t matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBu8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int4b_t matrix data from Global Memory to L0B Buffer by start block ID, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0b plus asc_sync.
 *
 * @override_id asc_copy_gm2l0b_sync__PCBi4x2_PGi4x2_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] base_idx 以16*16个数对矩阵进行分块，搬运的起始分块ID。
 * @param [in] repeat 迭代次数。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_gm2l0b_sync(
    __cb__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of bfloat16_t data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAbf16_PGbf16_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of half data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAf16_PGf16_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of float data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAf32_PGf32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int32_t data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAi32_PGi32_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint8_t data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAu8_PGu8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int8_t data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAi8_PGi8_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of uint32_t data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAu32_PGu32_u16_u8_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of int4b_t data from GM to L0A Buffer by fractal base index, repeat count, source stride and destination gap; replaced by asc_copy_gm2l0a plus asc_sync.
 *
 * @override_id asc_copy_gm2l0a_sync__PCAi4x2_PGi4x2_u16_u8_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] base_idx 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。
 */
__aicore__ inline void asc_copy_gm2l0a_sync(
    __ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to an L1 Buffer half destination; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1f16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to an L1 Buffer bfloat16_t destination; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1bf16_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to an L1 Buffer int8_t destination; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1i8_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to an L1 Buffer half destination; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1f16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to an L1 Buffer int16_t destination; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1i16_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to an L1 Buffer int8_t destination; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1i8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to an L1 Buffer uint8_t destination; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1u8_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split,
    bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes FP32 L0C matrix results and copies them to an L1 Buffer destination addressed by void* dst; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1v_PCCf32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous wrapper that quantizes int32_t L0C matrix results and copies them to an L1 Buffer destination addressed by void* dst; use asc_copy_l0c2l1 with asc_sync instead.
 *
 * @override_id asc_copy_l0c2l1_sync__PL1v_PCCi32_u16_u16_u32_u16_u8_u64_u8_b_b
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_size 源NZ矩阵在N方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 4095]； &bull; 开启NZ2ND功能，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &bull; 不开启NZ2ND功能，目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &bull; 开启NZ2ND/NZ2DN功能，目的ND矩阵每一行中的元素个数，取值不为0 ，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T), T为src的数据类型)。
 * @param [in] unit_flag_mode 与unit_flag参数相关，取值如下： &bull; 0保留值； &bull; 2 开启unit_flag，硬件执行完指令之后，不会设置寄存器； &bull; 3 开启unit_flag，硬件执行完指令后，会将unit_flag关闭。
 * @param [in] quant_pre 量化参数。取值见[功能说明](../cube_datamove/cube_compute_store/asc_copy_l0c2l1_arch_2201.md#功能说明)。
 * @param [in] relu_pre 开启relu。
 * @param [in] enable_channel_split 是否开启通道拆分的功能，默认false，不开启该功能。仅在src和dst都为float时才能开启通道拆分，且不能同时开启channel_split和NZ2ND功能。
 * @param [in] enable_nz2nd 开启NZ2ND开关。 &bull; false：不开启； &bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of left-matrix MX scale factors from L1 Buffer src to L0A_MX Buffer dst in 32-byte fp8_e8m0_t fractals; use asc_copy_l12l0a_mx plus asc_sync.
 *
 * @override_id asc_copy_l12l0a_mx_sync__u64_PL1f8e8m0_u16_u16_u8_u8_u16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_mx and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of right-matrix MX scale factors from L1 Buffer src to L0B_MX Buffer dst in 32-byte fp8_e8m0_t 2x16 fractals; use asc_copy_l12l0b_mx plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_mx_sync__u64_PL1f8e8m0_u16_u16_u8_u8_u16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: bfloat16_t (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp8_e4m3fn_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp8_e5m2_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: half (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: float (b32) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: hifloat8_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int16_t (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int32_t (b32) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int8_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: uint16_t (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: uint32_t (b32) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: uint8_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int4b_t (b4) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp4x2_e2m1_t (b4) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp4x2_e1m2_t (b4) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: bfloat16_t (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp8_e4m3fn_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp8_e5m2_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: half (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: float (b32) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: hifloat8_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int16_t (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int32_t (b32) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int8_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: uint16_t (b16) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: uint32_t (b32) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: uint8_t (b8) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int4b_t (b4) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp4x2_e2m1_t (b4) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp4x2_e1m2_t (b4) 2D fractals, L1 Buffer src to L0A Buffer dst; replaced by asc_copy_l12l0a_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0a_transpose_sync__PCAf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: bfloat16_t (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp8_e4m3fn_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp8_e5m2_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: half (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: float (b32) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: hifloat8_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int16_t (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int32_t (b32) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int8_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: uint16_t (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: uint32_t (b32) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: uint8_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: int4b_t (b4) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp4x2_e2m1_t (b4) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy: fp4x2_e1m2_t (b4) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: bfloat16_t (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBbf16_PL1bf16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp8_e4m3fn_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf8e4m3_PL1f8e4m3_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp8_e5m2_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf8e5m2_PL1f8e5m2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: half (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf16_PL1f16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: float (b32) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf32_PL1f32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: hifloat8_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBhf8_PL1hf8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int16_t (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi16_PL1i16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int32_t (b32) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi32_PL1i32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int8_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi8_PL1i8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: uint16_t (b16) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBu16_PL1u16_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: uint32_t (b32) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBu32_PL1u32_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: uint8_t (b8) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBu8_PL1u8_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: int4b_t (b4) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBi4x2_PL1i4x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp4x2_e2m1_t (b4) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf4e2m1x2_PL1f4e2m1x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync transpose copy: fp4x2_e1m2_t (b4) 2D fractals, L1 Buffer src to L0B Buffer dst; replaced by asc_copy_l12l0b_transpose plus asc_sync; writes result to dst and returns void; AIC only.
 *
 * @override_id asc_copy_l12l0b_transpose_sync__PCBf4e1m2x2_PL1f4e1m2x2_u16_u16_u8_u8_i16_u16
 * @archs 3510
 */
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes bfloat16_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBbf16_PL1bf16_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes fp8_e4m3fn_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf8e4m3_PL1f8e4m3_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes fp8_e5m2_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf8e5m2_PL1f8e5m2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes half 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf16_PL1f16_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes float 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf32_PL1f32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes hifloat8_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBhf8_PL1hf8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes int32_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBi32_PL1i32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes int8_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBi8_PL1i8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes uint32_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBu32_PL1u32_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes uint8_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBu8_PL1u8_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes fp4x2_e1m2_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf4e1m2x2_PL1f4e1m2x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes fp4x2_e2m1_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBf4e2m1x2_PL1f4e2m1x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b_trans and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload that transposes int4b_t 512-byte 2D fractals while copying from L1 Buffer src to L0B Buffer dst; use asc_copy_l12l0b_trans and asc_sync instead (void; result in dst).
 *
 * @override_id asc_copy_l12l0b_trans_sync__PCBi4x2_PL1i4x2_u16_u8_u16_u16_u16_u16
 * @archs 3510
 * @param [out] dst 目的L0B Buffer地址。
 * @param [in] src 源L1 Buffer地址。
 * @param [in] index_id 矩阵ID，搬运起始位置为源操作数中第几个分形。取值范围：[0, 65535]。
 * @param [in] repeat 迭代次数。  对于位宽为4的数据类型，每次迭代处理4个分形，每个分形为16\*64\*0.5B数据；  对于位宽为8的数据类型，每次迭代处理2个分形，每个分形为16\*32\*1B数据； 对于位宽为16的数据类型，每次迭代处理1个分形，每个分形为16\*16\*2B数据；对于位宽为32的数据类型，每次迭代处理4个分形，每个分形为16\*8\*4B数据。取值范围：[1, 255]。
 * @param [in] src_stride 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔。单位为单个分形512B。取值范围：[0, 65535]。
 * @param [in] dst_frac_gap 每个迭代内目的操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 * @param [in] src_frac_gap 每个迭代内源操作数转置前一个分形结束地址与后一个分形起始地址的间隔。单位为512B。仅在数据位宽为4、8或32时有效。
 */
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous copy of data from L1 Buffer src to Global Memory dst in bursts (n_burst, len_burst in 32-byte units, src_gap/dst_gap); use asc_copy_l12gm plus asc_sync.
 *
 * @override_id asc_copy_l12gm_sync__PGv_PL1v_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位为32字节。
 */
__aicore__ inline void asc_copy_l12gm_sync(
    __gm__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0c and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous bfloat16_t matrix copy from L1 Buffer to L0C Buffer in n_burst blocks of len_burst (32B units) with src_gap and dst_gap; use asc_copy_l12l0c with asc_sync.
 *
 * @override_id asc_copy_l12l0c_sync__PCCbf16_PL1bf16_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0c and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half matrix copy from L1 Buffer to L0C Buffer in n_burst blocks of len_burst (32B units) with src_gap and dst_gap; use asc_copy_l12l0c with asc_sync.
 *
 * @override_id asc_copy_l12l0c_sync__PCCf16_PL1f16_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0c and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half matrix copy from L1 Buffer to L0C Buffer in n_burst blocks of len_burst (32B units) with src_gap and dst_gap; use asc_copy_l12l0c with asc_sync.
 *
 * @override_id asc_copy_l12l0c_sync__PCCf16_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0c and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-bfloat16_t matrix copy from L1 Buffer to L0C Buffer in n_burst blocks of len_burst (32B units) with src_gap and dst_gap; use asc_copy_l12l0c with asc_sync.
 *
 * @override_id asc_copy_l12l0c_sync__PCCbf16_PL1f32_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0c and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float matrix copy from L1 Buffer to L0C Buffer in n_burst blocks of len_burst (32B units) with src_gap and dst_gap; use asc_copy_l12l0c with asc_sync.
 *
 * @override_id asc_copy_l12l0c_sync__PCCf32_PL1f32_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数的起始地址。
 * @param [in] src 源操作数的起始地址。
 * @param [in] n_burst 待搬运的连续传输数据块个数。取值范围：[1, 4095]。
 * @param [in] len_burst 待搬运的每个连续传输数据块的长度，单位为32个字节。取值范围：[1, 65535]。
 * @param [in] src_gap 表示相邻迭代间，源操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 * @param [in] dst_gap 表示相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。
 */
__aicore__ inline void asc_copy_l12l0c_sync(
    __cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0c and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous int32_t matrix copy from L1 Buffer to L0C Buffer in n_burst blocks of len_burst (32B units) with src_gap and dst_gap; use asc_copy_l12l0c with asc_sync.
 *
 * @override_id asc_copy_l12l0c_sync__PCCi32_PL1i32_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0c and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous uint32_t matrix copy from L1 Buffer to L0C Buffer in n_burst blocks of len_burst (32B units) with src_gap and dst_gap; use asc_copy_l12l0c with asc_sync.
 *
 * @override_id asc_copy_l12l0c_sync__PCCu32_PL1u32_u16_u16_u16_u16
 * @archs 2201
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a fp8_e4m3fn_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf8e4m3_PL1f8e4m3_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a fp8_e5m2_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAf8e5m2_PL1f8e5m2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a hifloat8_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAhf8_PL1hf8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a int16_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAi16_PL1i16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0a and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a uint16_t NC1HWC0 block from L1 Buffer src into L0A Buffer dst in 512-byte fractals; use asc_copy_l12l0a plus asc_sync; AIC only.
 *
 * @override_id asc_copy_l12l0a_sync__PCAu16_PL1u16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，对整个目标矩阵进行转置。 &nbsp;&nbsp;&bull; `false`：不开启转置； &nbsp;&nbsp;&bull; `true`：开启转置。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位，一般设置为`false`。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a fp8_e4m3fn_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf8e4m3_PL1f8e4m3_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a fp8_e5m2_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBf8e5m2_PL1f8e5m2_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a hifloat8_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBhf8_PL1hf8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a int8_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi8_PL1i8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a uint8_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu8_PL1u8_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a int16_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBi16_PL1i16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12l0b and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous 3D img2col load: moves a uint16_t NC1HWC0 block from L1 Buffer src to L0B Buffer dst in 512-byte fractals with forced transpose; use asc_copy_l12l0b plus asc_sync.
 *
 * @override_id asc_copy_l12l0b_sync__PCBu16_PL1u16_u16_u16_u16_u16_u8_u8_u8_u8_u8_u8_b_b_b_b_u16
 * @archs 3510
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
 * @param [in] filter_size_w 是否在`filter_w`的基础上将卷积核width增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] filter_size_h 是否在`filter_h`的基础上将卷积核height增加256个元素。 &nbsp;&nbsp;&bull; `true`：增加； &nbsp;&nbsp;&bull; `false`：不增加。
 * @param [in] transpose 是否启用转置功能，本接口对写入L0B Buffer的分形矩阵自动执行转置，该参数被硬件忽略，传入true或false不影响转置行为。
 * @param [in] f_matrix_ctrl 3D数据搬运Feature Map的属性描述寄存器组选择位。 &nbsp;&nbsp;&bull; `true`：从右矩阵中获取Feature Map的属性描述，配合[asc_set_l13d_fmatrix_b](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix_b.md)、[asc_set_l12l0b_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0b_3d_padding.md)、[asc_set_l13d_rpt_b](../cube_datamove/cube_load_aux_config/asc_set_l3d_rpt_b.md)使用； &nbsp;&nbsp;&bull; `false`：从左矩阵中获取FeatureMap的属性描述，配合[asc_set_l13d_fmatrix](../cube_datamove/cube_load_aux_config/asc_set_l13d_fmatrix.md)、[asc_set_l12l0a_3d_padding](../cube_datamove/cube_load_aux_config/asc_set_l12l0a_3d_padding.md)、[asc_set_l13d_rpt](../cube_datamove/cube_load_aux_config/asc_set_l13d_rpt.md)使用。
 * @param [in] channel_size 源操作数的通道数N。 对于b32类型，`channel_size`除8的余数应当为0或4，最大值可取为65532。 对于b16类型，`channel_size`除16的余数应当为0，4或8，最大值可取为65528。 对于b8类型，`channel_size`除32的余数应当为0，4，8或16，最大值可取为65520。
 */
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies bfloat16_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1bf16_PGbf16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies half data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1f16_PGf16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies float data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1f32_PGf32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies hifloat8_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1hf8_PGhf8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies int16_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1i16_PGi16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies int32_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1i32_PGi32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies int8_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1i8_PGi8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies uint16_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1u16_PGu16_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies uint32_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1u32_PGu32_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies uint8_t data from GM src to L1 Buffer dst with non-32B-aligned padding (Normal/Compact or left/right); result written to dst.
 *
 * @override_id asc_copy_gm2l1_align_sync__PL1u8_PGu8_u32_u32_u8_u8_b_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。源地址需1字节对齐。
 * @param [in] n_burst 待搬运的连续数据块个数。取值范围：[1, $2^{21}-1$]。
 * @param [in] len_burst 待搬运的每个连续数据块的长度，单位字节。取值范围：[1, $2^{21}-1$]。需满足dtype字节对齐：dtype为`b16`类型时len_burst需为2的倍数，dtype为`b32`时len_burst需为4的倍数。
 * @param [in] left_padding_count 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] right_padding_count 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`b8`时取值范围：[0, 32]，dtype为`b16`时取值范围：[0, 16]，dtype为`b32`时取值范围：[0, 8]。
 * @param [in] data_select_bit 填充数据来源选择。 &nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素； &nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取[asc_set_gm2l1_pad](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_pad.md)预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时，硬件强制使用预先配置的常量填充值，本参数设置无效。
 * @param [in] l2_cache_ctl 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#l2_cache_ctl_values)。
 * @param [in] burst_src_stride 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围：[1, $2^{40}-1$]。
 * @param [in] burst_dst_stride 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围：[1, $2^{21}-1$]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。
 */
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies bfloat16_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1bf16_PGbf16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies fp8_e4m3fn_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f8e4m3_PGf8e4m3_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies fp8_e5m2_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f8e5m2_PGf8e5m2_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies half data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f16_PGf16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies float data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1f32_PGf32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies hifloat8_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1hf8_PGhf8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies int16_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1i16_PGi16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies int32_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1i32_PGi32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies int8_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1i8_PGi8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies uint16_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1u16_PGu16_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies uint32_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1u32_PGu32_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_dn2nz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies uint8_t data from GM src to L1 Buffer dst converting DN to Nz with L2 cache control and smallc0 padding; result written to dst.
 *
 * @override_id asc_copy_gm2l1_dn2nz_sync__PL1u8_PGu8_u64_u8_u16_u32_u64_b
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] loop1_src_stride 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。
 * @param [in] l2_cache_ctl 配置输入的GM数据在L2 Cache中的管理策略。取值需为已定义的策略（0/1/2/4之一），其他值触发L2 Cache策略异常。取值说明见[表2](#l2_cache_ctl_values)。
 * @param [in] n_value 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。
 * @param [in] d_value 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。
 * @param [in] loop4_src_stride 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。 &nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复搬出源矩阵的第一个DN矩阵。
 * @param [in] smallc0_en SmallC0模式开关，仅当$d\_value \le 4$时可开启，否则导致搬运异常。 &nbsp;&nbsp;&bull; `false`：不开启SmallC0模式，按标准Nz格式的C0（32字节）排布搬运； &nbsp;&nbsp;&bull; `true`：开启SmallC0模式，搬运过程中搬运的数据量不满足$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐时，硬件在目的矩阵中自动补0至$4 \times \mathrm{sizeof}(\mathrm{dtype})$字节对齐。
 */
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous variant copies data from GM src to L1 Buffer dst by high-dimensional slicing with uint8_t pad_func_mode; replaced by non-sync asc_copy_gm2l1 plus asc_sync.
 *
 * @override_id asc_copy_gm2l1_sync__PL1v_PGv_u32_u32_u8_u64_u32
 * @archs 3510
 * @param [out] dst 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。
 * @param [in] src 源操作数，存储位置为GM。起始地址需要按照1字节对齐。
 * @param [in] n_burst 搬运的数据块的个数。取值范围：[1, $2^{17}-1$]。
 * @param [in] len_burst 搬运的每个数据块长度，单位32字节。取值范围：[1, $2^{17}-1$]。
 * @param [in] pad_func_mode 搬运过程中的填充或删除数据模式，取值范围：[0, 8]。填充模式是指在目的的数据中插入padding值，padding值需要通过[asc_set_gm2l1_padding](../cube_datamove/cube_load_aux_config/asc_set_gm2l1_padding.md)设置。删除模式是指在从搬运的每个32字节中移除部分数据。 &nbsp;&nbsp;&bull; `0`：不做padding； &nbsp;&nbsp;&bull; `1`：每有1个字节的数据，插入31个padding值，其中插入的每个padding的位宽为b8； &nbsp;&nbsp;&bull; `2`：每有2个字节的数据，插入15个padding值，其中插入的每个padding的位宽为b16； &nbsp;&nbsp;&bull; `3`：每有4个字节的数据，插入14个padding值，其中插入的每个padding的位宽为b16； &nbsp;&nbsp;&bull; `4`：每有8个字节的数据，插入12个padding值，其中插入的每个padding的位宽为b16；  &nbsp;&nbsp;&bull; `5`：每有16个字节的数据，插入8个padding值，其中插入的每个padding的位宽为b16； &nbsp;&nbsp;&bull; `6`：每有32个字节的数据，移除28个最高有效位数据，只保留4个最低有效位字节； &nbsp;&nbsp;&bull; `7`：每有32个字节的数据，移除24个最高有效位数据，只保留8个最低有效位字节； &nbsp;&nbsp;&bull; `8`：每有32个字节的数据，移除16个最高有效位数据，只保留16个最低有效位字节。
 * @param [in] src_stride 源操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{36}-1$]，单位32字节。 &nbsp;&nbsp;&bull; n_burst = 1时，`src_stride`无意义，设置为0即可。
 * @param [in] dst_stride 目的操作数相邻数据块之间的间隔（即前一个数据块起始地址与后一个数据块起始地址的差值），取值范围：[0, $2^{17}-1$]，单位32字节。 &nbsp;&nbsp;&bull; n_burst = 1时，`dst_stride`无意义，设置为0即可。
 */
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy writing MMAD Bias data from bfloat16_t L1 Buffer src into BiasTable Buffer dst (void return, size in bytes); use non-sync asc_copy_l12bt plus asc_sync.
 *
 * @override_id asc_copy_l12bt_sync__u64_PL1bf16_u32
 * @archs 3510
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy writing MMAD Bias data from half L1 Buffer src into BiasTable Buffer dst (void return, size in bytes); use non-sync asc_copy_l12bt plus asc_sync.
 *
 * @override_id asc_copy_l12bt_sync__u64_PL1f16_u32
 * @archs 3510
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy writing MMAD Bias data from float L1 Buffer src into BiasTable Buffer dst (void return, size in bytes); use non-sync asc_copy_l12bt plus asc_sync.
 *
 * @override_id asc_copy_l12bt_sync__u64_PL1f32_u32
 * @archs 3510
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12bt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync copy writing MMAD Bias data from int32_t L1 Buffer src into BiasTable Buffer dst (void return, size in bytes); use non-sync asc_copy_l12bt plus asc_sync.
 *
 * @override_id asc_copy_l12bt_sync__u64_PL1i32_u32
 * @archs 3510
 * @param [out] dst 目的操作数起始地址。
 * @param [in] src 源操作数起始地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l12ub and asc_sync)
__aicore__ inline void asc_copy_l12ub_sync(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst,
    uint16_t src_gap, uint16_t dst_gap);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload copying the float L0C Buffer result to L1 Buffer dst as int4b_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1_sync__PL1i4x2_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] l2_cache_mode 无效参数，用户无需关注，传入0即可。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 * @param [in] broadcast_en 无效参数，用户无需关注，传入false即可。
 * @param [in] enable_nz2dn 是否开启NZ2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync overload copying the int32_t L0C Buffer result to L1 Buffer dst as int4b_t with quantization/ReLU/format conversion; use asc_copy_l0c2l1 and asc_sync (void; result in dst).
 *
 * @override_id asc_copy_l0c2l1_sync__PL1i4x2_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 * @param [out] dst 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。
 * @param [in] src 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。
 * @param [in] n_size 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。
 * @param [in] m_size 源NZ矩阵在M方向上的大小。 &nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]； &nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。
 * @param [in] dst_stride &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。 &nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。
 * @param [in] src_stride 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。
 * @param [in] l2_cache_mode 无效参数，用户无需关注，传入0即可。
 * @param [in] enable_clip_relu_pre 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下： &nbsp;&nbsp;&bull; 0：不开启Clip ReLU； &nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。
 * @param [in] unit_flag_mode unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下： &nbsp;&nbsp;&bull; 0：不开启unit_flag； &nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位； &nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。 开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。
 * @param [in] quant_pre_mode 预处理阶段量化参数。取值见[功能说明](#功能说明)。
 * @param [in] relu_pre_mode 预处理阶段ReLU模式控制，取值如下： &nbsp;&nbsp;&bull; 0：不开启ReLU； &nbsp;&nbsp;&bull; 1：开启Normal ReLU； &nbsp;&nbsp;&bull; 2：开启Scalar ReLU； &nbsp;&nbsp;&bull; 3：开启Vector ReLU。
 * @param [in] enable_channel_split 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] enable_nz2nd 是否开启NZ2ND功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 * @param [in] quant_post 无效参数，用户无需关注，传入0即可。
 * @param [in] relu_post 无效参数，用户无需关注，传入0即可。
 * @param [in] clip_relu_post 无效参数，用户无需关注，传入false即可。
 * @param [in] eltwise_op 无效参数，用户无需关注，传入0即可。
 * @param [in] eltwise_antq_en 无效参数，用户无需关注，传入false或0即可。
 * @param [in] c0_pad_en 无效参数，用户无需关注，传入false即可。
 * @param [in] broadcast_en 无效参数，用户无需关注，传入false即可。
 * @param [in] enable_nz2dn 是否开启NZ2DN功能。 &nbsp;&nbsp;&bull; false：不开启； &nbsp;&nbsp;&bull; true：开启。
 */
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as bfloat16_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGbf16_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as half with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as fp8_e4m3fn_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGf8e4m3_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as hifloat8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGhf8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as int8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as uint8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGu8_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as float with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGf32_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as bfloat16_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGbf16_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as half with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGf16_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as int8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi8_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as uint8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGu8_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as int32_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi32_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to GM dst as int4b_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi4x2_PCCf32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to GM dst as int4b_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2gm_sync__PGi4x2_PCCi32_u16_u16_u32_u16_u8_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as bfloat16_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUbf16_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as half with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUf16_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as fp8_e4m3fn_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUf8e4m3_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as hifloat8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUhf8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as int8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUi8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as uint8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUu8_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as float with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUf32_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as bfloat16_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUbf16_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as half with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUf16_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as int8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUi8_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as uint8_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUu8_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as int32_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUi32_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer float data to UB dst as int4b_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUi4x2_PCCf32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_l0c2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync variant (dav-3510, AIC only): copies L0C Buffer int32_t data to UB dst as int4b_t with optional quantization, ReLU, Nz2ND/Nz2DN; result written to dst.
 *
 * @override_id asc_copy_l0c2ub_sync__PUi4x2_PCCi32_u16_u16_u32_u16_u8_b_u8_u8_u64_u8_b_b_u64_u8_b_u8_b_b_b_b
 * @archs 3510
 */
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of bfloat16_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1bf16_PGbf16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of float 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f32_PGf32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of fp8_e4m3fn_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f8e4m3_PGf8e4m3_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of fp8_e5m2_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f8e5m2_PGf8e5m2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of half 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f16_PGf16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of hifloat8_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1hf8_PGhf8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of int16_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1i16_PGi16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of int32_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1i32_PGi32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of int8_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1i8_PGi8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of uint16_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1u16_PGu16_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of uint32_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1u32_PGu32_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of uint8_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1u8_PGu8_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of fp4x2_e1m2_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f4e1m2x2_PGf4e1m2x2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of fp4x2_e2m1_t 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1f4e2m1x2_PGf4e2m1x2_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated void-returning sync copy of void* 2D GM-to-L1 fractals (512-byte units) from src to dst; use asc_copy_gm2l1_arch_3510 with asc_load_l2_cache_mode plus asc_sync; AIC only.
 *
 * @override_id asc_copy_gm2l1_sync__PL1v_PGv_u32_u32_u16_u16_u16_u8_u8
 * @archs 3510
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
