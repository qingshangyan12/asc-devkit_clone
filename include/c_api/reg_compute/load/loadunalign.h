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
#error "load/loadunalign.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADUNALIGN_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_LOAD_LOADUNALIGN_H
#define INCLUDE_C_API_REG_COMPUTE_LOAD_LOADUNALIGN_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/loadunalign_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned int8 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi8
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned uint8 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUu8
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned fp4x2_e2m1 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e2m1_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned fp4x2_e1m2 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e1m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned int4x2 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi4x2
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int4b_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned fp8_e8m0 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e8m0_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned fp8_e5m2 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e5m2_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned fp8_e4m3fn source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e4m3fn_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned hifloat8 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUhf8
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ hifloat8_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned int16 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi16
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned uint16 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUu16
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned half source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf16
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ half* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned bfloat16 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUbf16
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ bfloat16_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned int32 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi32
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned uint32 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUu32
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint32_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned float source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf32
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ float* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block around the dtype-aligned int64 source address src; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi64
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址，必须按dtype对齐。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int64_t* src);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi8_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUu8_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf4e2m1x2_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf4e1m2x2_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi4x2_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int4b_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf8e8m0_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf8e5m2_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf8e4m3_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUhf8_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ hifloat8_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi16_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUu16_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf16_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ half* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUbf16_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ bfloat16_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi32_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUu32_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint32_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUf32_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ float* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void preprocessing: initializes the unaligned register dst with the 32-byte UB prefix block at src plus the addr_reg element offset; no vector register is written.
 *
 * @override_id asc_loadunalign_pre__Rvloadunalign_PUi64_ar
 * @archs 3510
 * @param [out] dst 目的非对齐寄存器。作为后续非对齐搬入的前置数据缓存。
 * @param [in] src 源UB地址。`src`与`offset`共同确定的实际访问地址必须按dtype对齐。
 * @param [in] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。
 */
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int64_t* src, addr_reg offset);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length int8 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvi8_Rvloadunalign_PUi8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length uint8 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvu8_Rvloadunalign_PUu8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length int16 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvi16_Rvloadunalign_PUi16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length uint16 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvu16_Rvloadunalign_PUu16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length int32 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvi32_Rvloadunalign_PUi32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length uint32 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvu32_Rvloadunalign_PUu32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length half data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvf16_Rvloadunalign_PUf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_half& dst, vector_load_unalign& src0, __ubuf__ half* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length float data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvf32_Rvloadunalign_PUf32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_float& dst, vector_load_unalign& src0, __ubuf__ float* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length int64 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvi64_Rvloadunalign_PUi64
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length bfloat16 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvbf16_Rvloadunalign_PUbf16
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length fp8_e4m3fn data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvf8e4m3_Rvloadunalign_PUf8e4m3
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length hifloat8 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvhf8_Rvloadunalign_PUhf8
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length fp8_e5m2 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvf8e5m2_Rvloadunalign_PUf8e5m2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length fp8_e8m0 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvf8e8m0_Rvloadunalign_PUf8e8m0
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length fp4x2_e2m1 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvf4e2m1x2_Rvloadunalign_PUf4e2m1x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length fp4x2_e1m2 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvf4e1m2x2_Rvloadunalign_PUf4e1m2x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load: splices the 32-byte prefix cached in src0 with VL-length int4x2 data read from UB at src1 into dst; src1 is not updated.
 *
 * @override_id asc_loadunalign__Rvi4x2_Rvloadunalign_PUi4x2
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in] src0 非对齐寄存器，大小为32字节，保存本次搬入所需的前置数据缓存。
 * @param [in] src1 本次搬入的有效起始UB地址，必须按dtype对齐。本接口不会更新该指针。
 */
__simd_callee__ inline void asc_loadunalign(vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t* src1);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int8 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi8_Rvloadunalign_RPUi8_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length uint8 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvu8_Rvloadunalign_RPUu8_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int16 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi16_Rvloadunalign_RPUi16_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length uint16 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvu16_Rvloadunalign_RPUu16_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int32 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi32_Rvloadunalign_RPUi32_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length uint32 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvu32_Rvloadunalign_RPUu32_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length half data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf16_Rvloadunalign_RPUf16_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_half& dst, vector_load_unalign& src0, __ubuf__ half*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length float data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf32_Rvloadunalign_RPUf32_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_float& dst, vector_load_unalign& src0, __ubuf__ float*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int64 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi64_Rvloadunalign_RPUi64_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length bfloat16 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvbf16_Rvloadunalign_RPUbf16_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp8_e4m3fn data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf8e4m3_Rvloadunalign_RPUf8e4m3_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length hifloat8 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvhf8_Rvloadunalign_RPUhf8_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp8_e5m2 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf8e5m2_Rvloadunalign_RPUf8e5m2_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp4x2_e2m1 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf4e2m1x2_Rvloadunalign_RPUf4e2m1x2_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp4x2_e1m2 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf4e1m2x2_Rvloadunalign_RPUf4e1m2x2_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int4x2 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi4x2_Rvloadunalign_RPUi4x2_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp8_e8m0 data from UB at src1, splices it with the 32-byte prefix in src0, writes dst, then advances src1 by count elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf8e8m0_Rvloadunalign_RPUf8e8m0_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为更新后的`src1`所确定的实际读取地址所需的前置数据，可供下一次搬入使用。
 * @param [in,out] src1 源UB地址，实际读取地址必须按dtype对齐。
 * @param [in] count Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`增加`count × sizeof(dtype)`字节。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t*& src1, uint32_t count);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int8 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi8_Rvloadunalign_PUi8_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length uint8 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvu8_Rvloadunalign_PUu8_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int16 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi16_Rvloadunalign_PUi16_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length uint16 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvu16_Rvloadunalign_PUu16_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int32 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi32_Rvloadunalign_PUi32_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length uint32 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvu32_Rvloadunalign_PUu32_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length half data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf16_Rvloadunalign_PUf16_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_half& dst, vector_load_unalign& src0, __ubuf__ half* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length float data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf32_Rvloadunalign_PUf32_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_float& dst, vector_load_unalign& src0, __ubuf__ float* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int64 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi64_Rvloadunalign_PUi64_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length bfloat16 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvbf16_Rvloadunalign_PUbf16_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp8_e4m3fn data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf8e4m3_Rvloadunalign_PUf8e4m3_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length hifloat8 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvhf8_Rvloadunalign_PUhf8_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp8_e5m2 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf8e5m2_Rvloadunalign_PUf8e5m2_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp4x2_e2m1 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf4e2m1x2_Rvloadunalign_PUf4e2m1x2_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp4x2_e1m2 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf4e1m2x2_Rvloadunalign_PUf4e1m2x2_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length int4x2 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvi4x2_Rvloadunalign_PUi4x2_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t* src1, addr_reg& offset, uint32_t inc);

/**
 * @ingroup simd_c_api.reg_compute.load
 * @brief Void unaligned load with post-update: reads VL-length fp8_e8m0 data from UB at src1+offset, splices it with the 32-byte prefix in src0, writes dst, then advances offset by inc elements.
 *
 * @override_id asc_loadunalign_postupdate__Rvf8e8m0_Rvloadunalign_PUf8e8m0_Rar_u32
 * @archs 3510
 * @param [out] dst 目的矢量数据寄存器。dtype必须与`src1`一致，搬入VL长度数据。
 * @param [in,out] src0 非对齐寄存器，大小为32字节。本接口调用后，`src0`中缓存的数据更新为`src1`与更新后的`offset`所确定的实际读取地址所需的前置数据，可供当前循环迭代内的下一次搬入使用。
 * @param [in] src1 源UB基地址。该地址与`offset`共同确定实际读取地址，实际读取地址必须按dtype对齐。
 * @param [in,out] offset 地址寄存器，类型为`addr_reg`。该寄存器记录相对`src1`起始地址的偏移，单位为元素个数。`b8`、`b16`、`b32`模式分别表示每个元素占1字节、2字节、4字节。本接口完成搬入后会根据`inc`更新`offset`。
 * @param [in] inc Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的元素偏移量增加`inc`，即更新为`offset = offset + inc`。
 */
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t* src1, addr_reg& offset, uint32_t inc);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADUNALIGN_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_LOAD_LOADUNALIGN_H
#endif
