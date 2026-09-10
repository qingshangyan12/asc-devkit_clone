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
#include "impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi8_vi8_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst, vector_int8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu8_vu8_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_uint8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi16_vi16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int16_t* dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu16_vu16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi32_vi32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu32_vu32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf16_vf16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf32_vf32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUbf16_vbf16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUhf8_vhf8_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf4e2m1x2_vf4e2m1x2_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf4e1m2x2_vf4e1m2x2_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi4x2_vi4x2_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int4b_t* dst, vector_int4x2_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi8_vi8
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu8_vu8
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi16_vi16
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu16_vu16
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi32_vi32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu32_vu32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf16_vf16
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf32_vf32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUbf16_vbf16
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e4m3_vf8e4m3
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUhf8_vhf8
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e5m2_vf8e5m2
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e8m0_vf8e8m0
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf4e2m1x2_vf4e2m1x2
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf4e1m2x2_vf4e1m2x2
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi4x2_vi4x2
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi16_vi16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int16_t* dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu16_vu16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi32_vi32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu32_vu32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi64_vi64_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ int64_t* dst, vector_int64_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu64_vu64_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint64_t* dst, vector_uint64_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUf16_vf16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUf32_vf32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ float* dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUbf16_vbf16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUi32_vi32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUu32_vu32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUf32_vf32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi8_vi8_vi8
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu8_vu8_vu8
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi16_vi16_vi16
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu16_vu16_vu16
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi32_vi32_vi32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu32_vu32_vu32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf16_vf16_vf16
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ half* dst, vector_half src0, vector_half src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUbf16_vbf16_vbf16
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e4m3_vf8e4m3_vf8e4m3
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUhf8_vhf8_vhf8
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e5m2_vf8e5m2_vf8e5m2
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e8m0_vf8e8m0_vf8e8m0
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf4e2m1x2_vf4e2m1x2_vf4e2m1x2
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf4e1m2x2_vf4e1m2x2_vf4e1m2x2
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi4x2_vi4x2_vi4x2
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(__ubuf__ int4b_t* dst, vector_int4x2_t src0, vector_int4x2_t src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi8_vi8_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst, vector_int8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu8_vu8_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_uint8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi16_vi16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int16_t* dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu16_vu16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi32_vi32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu32_vu32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf16_vf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf32_vf32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUbf16_vbf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUhf8_vhf8_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf4e2m1x2_vf4e2m1x2_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf4e1m2x2_vf4e1m2x2_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi4x2_vi4x2_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int4b_t* dst, vector_int4x2_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi8_vi8_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu8_vu8_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi16_vi16_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu16_vu16_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi32_vi32_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu32_vu32_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf16_vf16_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf32_vf32_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUbf16_vbf16_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e4m3_vf8e4m3_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUhf8_vhf8_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e5m2_vf8e5m2_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e8m0_vf8e8m0_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf4e2m1x2_vf4e2m1x2_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf4e1m2x2_vf4e1m2x2_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi4x2_vi4x2_i32
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi16_vi16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int16_t* dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu16_vu16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint16_t* dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi32_vi32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu32_vu32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi64_vi64_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int64_t* dst, vector_int64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu64_vu64_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint64_t* dst, vector_uint64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUf16_vf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUf32_vf32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUbf16_vbf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUi32_vi32_i32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 相对`dst_align32b`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUu32_vu32_i32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 相对`dst_align32b`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUf32_vf32_i32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 相对`dst_align32b`起始地址的偏移量，类型为`int32_t`，单位为元素。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi8_vi8_vi8_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu8_vu8_vu8_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi16_vi16_vi16_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu16_vu16_vu16_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi32_vi32_vi32_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu32_vu32_vu32_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf16_vf16_vf16_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ half* dst, vector_half src0, vector_half src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUbf16_vbf16_vbf16_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e4m3_vf8e4m3_vf8e4m3_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUhf8_vhf8_vhf8_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e5m2_vf8e5m2_vf8e5m2_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e8m0_vf8e8m0_vf8e8m0_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf4e2m1x2_vf4e2m1x2_vf4e2m1x2_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf4e1m2x2_vf4e1m2x2_vf4e1m2x2_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi4x2_vi4x2_vi4x2_i32
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int4b_t* dst, vector_int4x2_t src0, vector_int4x2_t src1, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUi8_vi8_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int8_t*& dst, vector_int8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu8_vu8_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint8_t*& dst, vector_uint8_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUi16_vi16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu16_vu16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUi32_vi32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu32_vu32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUf16_vf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ half*& dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUf32_vf32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUbf16_vbf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUi8_vi8_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int8_t*& dst, vector_int8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUu8_vu8_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint8_t*& dst, vector_uint8_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUi16_vi16_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUu16_vu16_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUi32_vi32_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUu32_vu32_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUf16_vf16_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ half*& dst, vector_half src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUf32_vf32_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ float*& dst, vector_float src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st_postupdate__RPUbf16_vbf16_i32
 * @archs 3510
 * @brief 将源矢量数据寄存器中的第一个元素写入Unified Buffer（UB）中目的地址。单次搬出单个元素的数据，不支持配置掩码，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式和内容保持不变。接口采用Post Update模式，搬运完成后自动更新目的地址，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 地址偏移量，单位为元素个数。**接口执行后**将`dst`指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_storealign_1st_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUi16_vi16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUu16_vu16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUi32_vi32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUu32_vu32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUi64_vi64_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ int64_t*& dst, vector_int64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUu64_vu64_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ uint64_t*& dst, vector_uint64_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUf16_vf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ half*& dst, vector_half src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUf32_vf32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUbf16_vbf16_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, int32_t offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. Please use "
             "asc_storealign_pack_quarter_postupdate instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_postupdate_v2__RPUi32_vi32_i32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter_postupdate](../reg_compute/store/asc_storealign_pack_quarter_postupdate.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. Please use "
             "asc_storealign_pack_quarter_postupdate instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_postupdate_v2__RPUu32_vu32_i32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter_postupdate](../reg_compute/store/asc_storealign_pack_quarter_postupdate.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. Please use "
             "asc_storealign_pack_quarter_postupdate instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_postupdate_v2__RPUf32_vf32_i32_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter_postupdate](../reg_compute/store/asc_storealign_pack_quarter_postupdate.md)实现此功能。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate_v2(
    __ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi8_vi8_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ int8_t* dst, vector_int8_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu8_vu8_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint8_t* dst, vector_uint8_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi16_vi16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu16_vu16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi32_vi32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu32_vu32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf16_vf16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ half* dst, vector_half src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf32_vf32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUbf16_vbf16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUhf8_vhf8_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf4e2m1x2_vf4e2m1x2_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf4e1m2x2_vf4e1m2x2_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi4x2_vi4x2_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int4b_t* dst, vector_int4x2_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi8_vi8_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst, vector_int8_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu8_vu8_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint8_t* dst, vector_uint8_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi16_vi16_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu16_vu16_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi32_vi32_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUu32_vu32_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf16_vf16_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ half* dst, vector_half src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf32_vf32_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ float* dst, vector_float src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUbf16_vbf16_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e4m3_vf8e4m3_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUhf8_vhf8_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e5m2_vf8e5m2_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf8e8m0_vf8e8m0_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf4e2m1x2_vf4e2m1x2_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUf4e1m2x2_vf4e1m2x2_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_1st__PUi4x2_vi4x2_ar
 * @archs 3510
 * @brief 将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口不支持设置掩码，固定搬出矢量数据寄存器中第一个元素到目的地址处，目的地址需按`sizeof(dtype)`对齐，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数的起始地址，类型为`__ubuf__ *`，起始地址需dtype对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。
 */
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int4b_t* dst, vector_int4x2_t src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi16_vi16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int16_t* dst, vector_int16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu16_vu16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint16_t* dst, vector_uint16_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi32_vi32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu32_vu32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUi64_vi64_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ int64_t* dst, vector_int64_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu64_vu64_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ uint64_t* dst, vector_uint64_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUf16_vf16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ half* dst, vector_half src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUf32_vf32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUbf16_vbf16_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, addr_reg offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUi32_vi32_ar_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 地址寄存器偏移，类型为`addr_reg`，由配套的地址寄存器构造接口预先构造。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ int32_t* dst_align32b, vector_int32_t src, addr_reg offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUu32_vu32_ar_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 地址寄存器偏移，类型为`addr_reg`，由配套的地址寄存器构造接口预先构造。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ uint32_t* dst_align32b, vector_uint32_t src, addr_reg offset, vector_bool mask);

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. Please use asc_storealign_pack_quarter instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storealign_pack_v2__PUf32_vf32_ar_vb
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_storealign_pack_quarter](../reg_compute/store/asc_storealign_pack_quarter.md)实现此功能。
 * @param [out] dst_align32b 目的操作数的起始地址，需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。支持的数据类型为`int32_t`、`uint32_t`、`float`。
 * @param [in] offset 地址寄存器偏移，类型为`addr_reg`，由配套的地址寄存器构造接口预先构造。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。
 */
__simd_callee__ inline void asc_storealign_pack_v2(
    __ubuf__ float* dst_align32b, vector_float src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi8_vi8_vi8_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int8_t* dst, vector_int8_t src0, vector_int8_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu8_vu8_vu8_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint8_t* dst, vector_uint8_t src0, vector_uint8_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi16_vi16_vi16_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int16_t* dst, vector_int16_t src0, vector_int16_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu16_vu16_vu16_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint16_t* dst, vector_uint16_t src0, vector_uint16_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi32_vi32_vi32_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int32_t* dst, vector_int32_t src0, vector_int32_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUu32_vu32_vu32_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ uint32_t* dst, vector_uint32_t src0, vector_uint32_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf16_vf16_vf16_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ half* dst, vector_half src0, vector_half src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUbf16_vbf16_vbf16_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src0, vector_bfloat16_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e4m3_vf8e4m3_vf8e4m3_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUhf8_vhf8_vhf8_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src0, vector_hifloat8_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e5m2_vf8e5m2_vf8e5m2_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf8e8m0_vf8e8m0_vf8e8m0_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf4e2m1x2_vf4e2m1x2_vf4e2m1x2_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src0, vector_fp4x2_e2m1_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUf4e1m2x2_vf4e1m2x2_vf4e1m2x2_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src0, vector_fp4x2_e1m2_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_intlv__PUi4x2_vi4x2_vi4x2_ar
 * @archs 3510
 * @brief 将两个矢量数据寄存器中的数据按元素顺序交织后连续对齐搬出到Unified Buffer（UB）。单次搬出量为`2 × VL`（共512字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src0 源操作数0（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] src1 源操作数1（矢量数据寄存器）。`dtype`须与`dst`一致。
 */
__simd_callee__ inline void asc_storealign_intlv(
    __ubuf__ int4b_t* dst, vector_int4x2_t src0, vector_int4x2_t src1, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi8_vi8_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int8_t* dst, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu8_vu8_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint8_t* dst, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi16_vi16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int16_t* dst, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu16_vu16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint16_t* dst, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUi32_vi32_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ int32_t* dst, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu32_vu32_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ uint32_t* dst, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUbf16_vbf16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ bfloat16_t* dst, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e4m3_vf8e4m3_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUhf8_vhf8_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ hifloat8_t* dst, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e5m2_vf8e5m2_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf8e8m0_vf8e8m0_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf16_vf16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ half* dst, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUf32_vf32_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] block_stride 目的操作数相邻DataBlock之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的操作数起始地址偏移，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign(
    __ubuf__ float* dst, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUi8_vi8_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int8_t*& dst, vector_int8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu8_vu8_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint8_t*& dst, vector_uint8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUi16_vi16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int16_t*& dst, vector_int16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu16_vu16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint16_t*& dst, vector_uint16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUi32_vi32_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu32_vu32_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUbf16_vbf16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_bfloat16_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUf8e4m3_vf8e4m3_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_fp8_e4m3fn_t src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUhf8_vhf8_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_hifloat8_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUf8e5m2_vf8e5m2_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_fp8_e5m2_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUf8e8m0_vf8e8m0_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_fp8_e8m0_t src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUf16_vf16_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ half*& dst, vector_half src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUf32_vf32_u16_u16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [in] block_stride 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。
 * @param [in] repeat_stride 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_postupdate(
    __ubuf__ float*& dst, vector_float src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu32_vb_ar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu32_vb_ar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu16_vb_ar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu16_vb_ar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu8_vb_ar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu8_vb_ar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst, vector_bool src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu16_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu8_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu8_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst, vector_bool src);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu32_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint32_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu32_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint32_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu16_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint16_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu16_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint16_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign__PUu8_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据以对齐的方式搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），将掩码寄存器中的数据直接搬出，不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数（矢量）的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 */
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack__PUu8_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB）。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 */
__simd_callee__ inline void asc_storealign_pack(__ubuf__ uint8_t* dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu32_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 */
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUu32_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint32_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu16_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 */
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUu16_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint16_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_postupdate__RPUu8_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续对齐搬出到Unified Buffer（UB），启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，单次搬出量为VL（256字节），支持配置掩码用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。当源操作数为掩码寄存器时，单次搬出量为掩码寄存器长度（32字节），不支持配置掩码。搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 */
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_postupdate__RPUu8_vb_i32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。当源操作数为矢量数据寄存器时，`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置。`mask`对应位置为0时，压缩后对应的目的位置保持原值。当源操作数为掩码寄存器时，每间隔1 bit丢弃1 bit，将保留的bit连续压缩存储到目的地址，掩码寄存器搬出场景不支持配置掩码。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 */
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint8_t*& dst, vector_bool src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUi32_vi32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ int32_t* dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUu32_vu32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUf32_vf32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(__ubuf__ float* dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUi32_vi32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ int32_t* dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUu32_vu32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ uint32_t* dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUf32_vf32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ float* dst, vector_float src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUi32_vi32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ int32_t* dst, vector_int32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUu32_vu32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ uint32_t* dst, vector_uint32_t src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter__PUf32_vf32_ar_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB）。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中每个32bit的有效元素中低8bit数据写入压缩后对应的目的位置，`mask`对应位置为0时，压缩后对应的目的位置保持原值。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，目的操作数为UB地址。
 * @param [out] dst 目的操作数的起始地址。类型为`__ubuf__ *`，起始地址需32字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter(
    __ubuf__ float* dst, vector_float src, addr_reg offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter_postupdate__RPUi32_vi32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中有效的32位元素中的低8bit数据写入压缩后对应的目的位置；`mask`对应位置为0时，对应元素不参与搬出，压缩后对应的目的位置保持原值。`offset`单位为元素。搬出完成后，目的地址累加`offset × sizeof(dtype)`字节。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ int32_t*& dst, vector_int32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter_postupdate__RPUu32_vu32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中有效的32位元素中的低8bit数据写入压缩后对应的目的位置；`mask`对应位置为0时，对应元素不参与搬出，压缩后对应的目的位置保持原值。`offset`单位为元素。搬出完成后，目的地址累加`offset × sizeof(dtype)`字节。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ uint32_t*& dst, vector_uint32_t src, int32_t offset, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storealign_pack_quarter_postupdate__RPUf32_vf32_i32_vb
 * @archs 3510
 * @brief 将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将`src`中有效的32位元素中的低8bit数据写入压缩后对应的目的位置；`mask`对应位置为0时，对应元素不参与搬出，压缩后对应的目的位置保持原值。`offset`单位为元素。搬出完成后，目的地址累加`offset × sizeof(dtype)`字节。本接口在Vector Function（`__simd_vf__`标记的函数）内使用。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] offset 目的地址更新量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。需通过掩码设置接口预先赋值后再传入。
 */
__simd_callee__ inline void asc_storealign_pack_quarter_postupdate(
    __ubuf__ float*& dst, vector_float src, int32_t offset, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREALIGN_H
#endif
