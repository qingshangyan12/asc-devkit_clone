/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "cache_ctrl.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CACHE_CTRL_CACHE_CTRL_H
#endif

#ifndef INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H
#define INCLUDE_C_API_CACHE_CTRL_CACHE_CTRL_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h"

#endif

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_icache_preload__PCv_i64
 * @archs 3510 2201
 * @brief 从指令所在GM地址预加载数据到对应的cacheline中。
 * @param [in] addr 预加载数据的地址。
 * @param [in] prefetch_len 预加载数据的长度，单位为2K Byte，取值需满足 $\text{prefetch\_len} < \dfrac{\text{ICache大小}}{2K}$。其中，AIC和AIV的ICache大小分别为32KB和16KB。
 */
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len);

namespace __asc_aicore {
/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_dcci_single__PGv
 * @archs 3510 2201
 * @brief 在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。
 * @param [in] dst 需要刷新缓存的向量的起始地址。
 */
__aicore__ inline void asc_dcci_single(__gm__ void* dst);
}

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_ub_dcci_single__PUv
 * @archs 3510 2201
 * @brief 在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。
 * @param [in] dst 需要刷新缓存的向量的起始地址。
 */
__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_dcci_entire_ub__z
 * @archs 3510 2201
 * @brief 在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。
 */
__aicore__ inline void asc_dcci_entire_ub();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_dcci_entire_out__z
 * @archs 3510 2201
 * @brief 在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。
 */
__aicore__ inline void asc_dcci_entire_out();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_dcci_entire_all__z
 * @archs 3510 2201
 * @brief 在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。
 */
__aicore__ inline void asc_dcci_entire_all();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_dcci_entire_atomic__z
 * @archs 3510 2201
 * @brief 在AI Core内部，Scalar单元和DMA单元都可能对Global Memory进行访问。
 */
__aicore__ inline void asc_dcci_entire_atomic();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_get_icache_preload_status__z
 * @archs 3510 2201
 * @brief 获取ICache的Preload的状态。
 */
__aicore__ inline int64_t asc_get_icache_preload_status();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_datacache_preload__PGu64_i64
 * @archs 3510 2201
 * @brief 从源地址所在的特定GM地址预加载数据到Data Cache中，每次调用只能预加载一个Cache Line大小的数据。
 * @param [in] address 源操作数的起始地址。
 * @param [in] offset 在源操作数上偏移offset大小开始加载数据，单位为Bytes。
 */
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_dci__z
 * @archs 3510
 * @brief 数据缓存失效，该指令用于使整个数据缓存无效化，且不会对处于“脏”状态的缓存行执行写回操作。换言之，所有缓存行将被直接标记为无效，其修改过但尚未同步至主存的数据将被丢弃。
 */
__aicore__ inline void asc_dci();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_set_scalar_cache_mode__loadl2
 * @archs 3510
 * @brief 配置标量单元访问Global Memory时的L2 Cache管理策略。
 * @param [in] l2_cache_mode L2 Cache管理策略。load重载传入[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)枚举值；store重载传入[asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md)枚举值。
 */
__aicore__ inline void asc_set_scalar_cache_mode(asc_load_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_set_scalar_cache_mode__storel2
 * @archs 3510
 * @brief 配置标量单元访问Global Memory时的L2 Cache管理策略。
 * @param [in] l2_cache_mode L2 Cache管理策略。load重载传入[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)枚举值；store重载传入[asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md)枚举值。
 */
__aicore__ inline void asc_set_scalar_cache_mode(asc_store_l2_cache_mode l2_cache_mode);

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_get_scalar_load_cache_mode__z
 * @archs 3510
 * @brief 读取当前标量读（load）路径的L2 Cache管理策略，并返回[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)枚举值。
 */
__aicore__ inline asc_load_l2_cache_mode asc_get_scalar_load_cache_mode();

/**
 * @ingroup simd_c_api.cache_ctrl
 * @override_id asc_get_scalar_store_cache_mode__z
 * @archs 3510
 * @brief 读取当前标量写（store）路径的L2 Cache管理策略，并返回[asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md)枚举值。
 */
__aicore__ inline asc_store_l2_cache_mode asc_get_scalar_store_cache_mode();

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CACHE_CTRL_CACHE_CTRL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_CACHE_CTRL_CACHE_CTRL_H
#endif
