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
#error "scatter/ub_scatter.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_SCATTER_UB_SCATTER_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_SCATTER_UB_SCATTER_H
#define INCLUDE_C_API_REG_COMPUTE_SCATTER_UB_SCATTER_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h"
#endif

[[deprecated("NOTICE: asc_scatter(vector_int8_t& src, __ubuf__ int8_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int8_t* dst, vector_int8_t src, vector_uint16_t index, vector_bool mask) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvi8_PUi8_vu16_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_scatter(vector_int8_t& src, __ubuf__ int8_t* dst, vector_uint16_t index, vector_bool mask);

[[deprecated("NOTICE: asc_scatter(vector_uint8_t& src, __ubuf__ uint8_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ uint8_t* dst, vector_uint8_t src, vector_uint16_t index, vector_bool "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvu8_PUu8_vu16_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint8_t& src, __ubuf__ uint8_t* dst, vector_uint16_t index, vector_bool mask);

[[deprecated("NOTICE: asc_scatter(vector_int16_t& src, __ubuf__ int16_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int16_t* dst, vector_int16_t src, vector_uint16_t index, vector_bool "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvi16_PUi16_vu16_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_int16_t& src, __ubuf__ int16_t* dst, vector_uint16_t index, vector_bool mask);

[[deprecated("NOTICE: asc_scatter(vector_uint16_t& src, __ubuf__ uint16_t* dst, vector_uint16_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_uint16_t index, vector_bool "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvu16_PUu16_vu16_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint16_t& src, __ubuf__ uint16_t* dst, vector_uint16_t index, vector_bool mask);

[[deprecated("NOTICE: asc_scatter(vector_int32_t& src, __ubuf__ int32_t* dst, vector_uint32_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int32_t* dst, vector_int32_t src, vector_uint32_t index, vector_bool "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvi32_PUi32_vu32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_int32_t& src, __ubuf__ int32_t* dst, vector_uint32_t index, vector_bool mask);

[[deprecated("NOTICE: asc_scatter(vector_uint32_t& src, __ubuf__ uint32_t* dst, vector_uint32_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_uint32_t index, vector_bool "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvu32_PUu32_vu32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint32_t& src, __ubuf__ uint32_t* dst, vector_uint32_t index, vector_bool mask);

[[deprecated("NOTICE: asc_scatter(vector_bfloat16_t& src, __ubuf__ bfloat16_t* dst, vector_uint16_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_uint16_t index, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvbf16_PUbf16_vu16_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "vector_bool mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_bfloat16_t& src, __ubuf__ bfloat16_t* dst, vector_uint16_t index, vector_bool mask);

[[deprecated(
    "NOTICE: asc_scatter(vector_half& src, __ubuf__ half* dst, vector_uint16_t index, vector_bool mask) is deprecated. "
    "Please use asc_scatter(__ubuf__ half* dst, vector_half src, vector_uint16_t index, vector_bool mask) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvf16_PUf16_vu16_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
    "instead.")]] __simd_callee__ inline void
asc_scatter(vector_half& src, __ubuf__ half* dst, vector_uint16_t index, vector_bool mask);

[[deprecated("NOTICE: asc_scatter(vector_float& src, __ubuf__ float* dst, vector_uint32_t index, vector_bool mask) is "
             "deprecated. "
             "Please use asc_scatter(__ubuf__ float* dst, vector_float src, vector_uint32_t index, vector_bool mask) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_scatter__Rvf32_PUf32_vu32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_scatter](../reg_compute/scatter/asc_scatter.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] index 数据索引（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_scatter(vector_float& src, __ubuf__ float* dst, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUi8_vi8_vu16_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(
    __ubuf__ int8_t* dst, vector_int8_t src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUu8_vu8_vu16_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(
    __ubuf__ uint8_t* dst, vector_uint8_t src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUi16_vi16_vu16_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(
    __ubuf__ int16_t* dst, vector_int16_t src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUu16_vu16_vu16_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(
    __ubuf__ uint16_t* dst, vector_uint16_t src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUi32_vi32_vu32_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(
    __ubuf__ int32_t* dst, vector_int32_t src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUu32_vu32_vu32_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(
    __ubuf__ uint32_t* dst, vector_uint32_t src, vector_uint32_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUbf16_vbf16_vu16_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUf16_vf16_vu16_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(__ubuf__ half* dst, vector_half src, vector_uint16_t index, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.scatter
 * @override_id asc_scatter__PUf32_vf32_vu32_vb
 * @archs 3510
 * @brief 根据索引寄存器中的索引值，将源矢量数据寄存器中的元素分散搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，掩码为1的源元素写入`dst`中对应索引位置，掩码为0的源元素不写入目的地址，保留目的地址原数据。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器），数据类型须与`dst`指向的数据类型一致。
 * @param [in] index 数据索引（矢量数据寄存器），单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与搬运。对应位置为1时参与计算，为0时不参与计算，保留目的地址原数据。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_scatter(__ubuf__ float* dst, vector_float src, vector_uint32_t index, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_SCATTER_UB_SCATTER_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_SCATTER_UB_SCATTER_H
#endif
