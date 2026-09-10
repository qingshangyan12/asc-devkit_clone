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
#error "store/storeunalign.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREUNALIGN_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_STORE_STOREUNALIGN_H
#define INCLUDE_C_API_REG_COMPUTE_STORE_STOREUNALIGN_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUi8_Rvstoreunalign_vi8_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUu8_Rvstoreunalign_vu8_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUi16_Rvstoreunalign_vi16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUu16_Rvstoreunalign_vu16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUi32_Rvstoreunalign_vi32_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUi64_Rvstoreunalign_vi64_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUu32_Rvstoreunalign_vu32_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUf16_Rvstoreunalign_vf16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUf32_Rvstoreunalign_vf32_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUbf16_Rvstoreunalign_vbf16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUf8e4m3_Rvstoreunalign_vf8e4m3_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUhf8_Rvstoreunalign_vhf8_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUf8e5m2_Rvstoreunalign_vf8e5m2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUf8e8m0_Rvstoreunalign_vf8e8m0_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUf4e2m1x2_Rvstoreunalign_vf4e2m1x2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUf4e1m2x2_Rvstoreunalign_vf4e1m2x2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign__PUi4x2_Rvstoreunalign_vi4x2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器中的前`count`个元素连续搬出到非32字节对齐的Unified Buffer（UB）地址。`unalign_reg` 为非对齐寄存器，按引用传入并在本接口调用后被更新为本次搬出尾部数据，供下一次本接口或非对齐搬出收尾接口继续拼接使用。每次调用后，用户需手动更新目的操作数在UB中的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] src 源操作数（矢量数据寄存器）。数据类型须与`dst`一致。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`，最大值不超过单个矢量数据寄存器可容纳的元素个数。
 */
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUi8_Rvstoreunalign_vi8_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t*& dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUu8_Rvstoreunalign_vu8_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUi16_Rvstoreunalign_vi16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t*& dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUu16_Rvstoreunalign_vu16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUi32_Rvstoreunalign_vi32_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t*& dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUu32_Rvstoreunalign_vu32_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUi64_Rvstoreunalign_vi64_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t*& dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUf16_Rvstoreunalign_vf16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half*& dst, vector_store_unalign& src0, vector_half src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUf32_Rvstoreunalign_vf32_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float*& dst, vector_store_unalign& src0, vector_float src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUbf16_Rvstoreunalign_vbf16_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUf8e4m3_Rvstoreunalign_vf8e4m3_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUhf8_Rvstoreunalign_vhf8_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUf8e5m2_Rvstoreunalign_vf8e5m2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUf8e8m0_Rvstoreunalign_vf8e8m0_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUf4e2m1x2_Rvstoreunalign_vf4e2m1x2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUf4e1m2x2_Rvstoreunalign_vf4e1m2x2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUi4x2_Rvstoreunalign_vi4x2_u32
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t*& dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUu16_Rvstoreunalign_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__RPUu32_Rvstoreunalign_vb
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_bool src1);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUi8_Rvstoreunalign_vi8_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUu8_Rvstoreunalign_vu8_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUi16_Rvstoreunalign_vi16_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUu16_Rvstoreunalign_vu16_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUi32_Rvstoreunalign_vi32_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUu32_Rvstoreunalign_vu32_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUi64_Rvstoreunalign_vi64_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUf16_Rvstoreunalign_vf16_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUf32_Rvstoreunalign_vf32_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUbf16_Rvstoreunalign_vbf16_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUf8e4m3_Rvstoreunalign_vf8e4m3_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUhf8_Rvstoreunalign_vhf8_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUf8e5m2_Rvstoreunalign_vf8e5m2_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUf8e8m0_Rvstoreunalign_vf8e8m0_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUf4e2m1x2_Rvstoreunalign_vf4e2m1x2_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUf4e1m2x2_Rvstoreunalign_vf4e1m2x2_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_postupdate__PUi4x2_Rvstoreunalign_vi4x2_Rar
 * @archs 3510
 * @brief 将矢量数据寄存器或掩码寄存器中的数据连续搬出到非32字节对齐的Unified Buffer（UB）地址，启用Post Update模式在搬出后自动累加目的地址指针或地址寄存器，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__` 标记的函数）内使用。
 * @param [out] dst 目的操作数在UB中的起始地址，类型为`__ubuf__ *`。
 * @param [in] count 本次搬出的元素个数，类型为`uint32_t`。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, addr_reg& count);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi8_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUu8_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf4e2m1x2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf4e1m2x2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi4x2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf8e8m0_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf8e5m2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf8e4m3_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUhf8_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ hifloat8_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUu16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUbf16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi32_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUu32_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf32_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi64_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi8_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUu8_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf4e2m1x2_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf4e1m2x2_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi4x2_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf8e8m0_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf8e5m2_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf8e4m3_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUhf8_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ hifloat8_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi16_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUu16_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf16_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUbf16_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi32_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUu32_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUf32_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post__PUi64_vstoreunalign_ar
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于连续非对齐搬运收尾。
 * @param [out] dst 目的操作数在UB中的基地址，类型为`__ubuf__ *`。地址无需32字节对齐，但须按`sizeof(dtype)`字节对齐。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序主搬出接口使用同一个寄存器。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUi8_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int8_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUu8_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUf4e2m1x2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUf4e1m2x2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUi4x2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int4b_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUf8e8m0_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUf8e5m2_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUf8e4m3_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUhf8_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUi16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int16_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUu16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUf16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ half*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUbf16_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUi32_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int32_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUu32_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUf32_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ float*& dst, vector_store_unalign src, int32_t offset);

/**
 * @ingroup simd_c_api.reg_compute.store
 * @override_id asc_storeunalign_post_postupdate__RPUi64_vstoreunalign_i32
 * @archs 3510
 * @brief 将非对齐寄存器中暂存的尾块写入Unified Buffer（UB），用于搬出[asc_storeunalign_postupdate立即数偏移搬出模式](asc_storeunalign_postupdate.md)产生的非对齐寄存器。启用Post Update模式在搬出后自动累加目的地址指针，便于硬件循环内连续多次调用时无需手动维护目的地址。搬出结束后，目的地址向后偏移立即数`offset`个元素。
 * @param [in] unalign_reg 非对齐寄存器，类型为`vector_store_unalign`，长度为32字节。须与前序接口使用同一个寄存器。
 * @param [in] offset 偏移量，类型为`int32_t`，单位为元素。指令搬出后，`dst`向后偏移`offset`个元素，若`dst`已指向结束地址时设置为0。
 */
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int64_t*& dst, vector_store_unalign src, int32_t offset);

[[deprecated("NOTICE: asc_storeunalign_postupdate with a b8 destination and vector_bool source is deprecated. "
             "Please use the b16 or b32 overload instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__RPUu8_Rvstoreunalign_vb
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_bool src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUi8_Rvstoreunalign_vi8
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUu8_Rvstoreunalign_vu8
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUi16_Rvstoreunalign_vi16
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUu16_Rvstoreunalign_vu16
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUi32_Rvstoreunalign_vi32
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUu32_Rvstoreunalign_vu32
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUi64_Rvstoreunalign_vi64
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUf16_Rvstoreunalign_vf16
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ half* dst, vector_store_unalign& src0, vector_half src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUf32_Rvstoreunalign_vf32
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ float* dst, vector_store_unalign& src0, vector_float src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUbf16_Rvstoreunalign_vbf16
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUf8e4m3_Rvstoreunalign_vf8e4m3
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUf8e5m2_Rvstoreunalign_vf8e5m2
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUf8e8m0_Rvstoreunalign_vf8e8m0
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUf4e2m1x2_Rvstoreunalign_vf4e2m1x2
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUf4e1m2x2_Rvstoreunalign_vf4e1m2x2
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1);

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_postupdate__PUi4x2_Rvstoreunalign_vi4x2
 * @archs 3510
 * @brief **该接口不指定存储偏移量的原型已废弃，请使用[asc_squeeze_and_storeunalign](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)实现此功能。掩码寄存器搬出模式的b8数据类型原型已废弃，请使用b16或b32数据类型原型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 */
             "instead.")]] __simd_callee__ inline void
asc_storeunalign_postupdate(__ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUi8_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUu8_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUf4e2m1x2_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUf4e1m2x2_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUi4x2_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUf8e8m0_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUf8e5m2_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUf8e4m3_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUi16_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUu16_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUf16_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUbf16_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUi32_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUu32_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUf32_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src);

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_storeunalign_post__PUi64_vstoreunalign
 * @archs 3510
 * @brief **该接口已废弃。请使用[asc_squeeze_and_storeunalign_finalize](../reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)实现此功能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 非对齐寄存器，用于保存非对齐数据，长度32B。
 */
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREUNALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_STORE_STOREUNALIGN_H
#endif
