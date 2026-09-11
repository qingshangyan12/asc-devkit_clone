/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_scalar_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_scalar_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SCALAR_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_SCALAR_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_SCALAR_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/utils/kernel_utils_mode.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
template <int countValue>
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id GetBitCount__u64
 * @archs 3510 2201 2002 x90 9030
 * @brief 统计uint64\_t类型数值的二进制表示中指定比特值（0或1）的出现次数。
 */
__aicore__ inline int64_t GetBitCount(uint64_t valueIn);

// ScalarGetCountOfValue has been updated, please use GetBitCount instead.
template <int countValue>
__aicore__ inline int64_t ScalarGetCountOfValue(uint64_t valueIn);

/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id CountLeadingZero__u64
 * @archs 3510 2201 2002 x90 9030
 * @brief 统计uint64\_t类型数值的二进制表示中前导0的个数（二进制表示中最高有效位到第一个'1'的'0'的数量，输入为0时返回64）。
 */
__aicore__ inline int64_t CountLeadingZero(uint64_t valueIn);

// ScalarCountLeadingZero has been updated, please use CountLeadingZero instead.
__aicore__ inline int64_t ScalarCountLeadingZero(uint64_t valueIn);

template <typename T>
__aicore__ inline void GetUintDivMagicAndShift(T& magic, T& shift, T divisor);

/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id CountBitsCntSameAsSignBit__i64
 * @archs 3510 2201 2002 x90 9030
 * @brief 统计int64\_t类型数值的二进制表示中，从最高数值位开始与符号位相同的连续比特位的个数。
 */
__aicore__ inline int64_t CountBitsCntSameAsSignBit(int64_t valueIn);

template <int countValue>
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id GetSFFValue__u64
 * @archs 3510 2201 2002 x90 9030
 * @brief 查找uint64\_t类型数值的二进制表示中从最低有效位开始的首个指定比特值（0或1）的位置，如果没找到则返回-1。
 */
__aicore__ inline int64_t GetSFFValue(uint64_t valueIn);

// ScalarGetSFFValue has been updated, please use GetSFFValue instead.
template <int countValue>
__aicore__ inline int64_t ScalarGetSFFValue(uint64_t valueIn);

template <typename T, typename U, RoundMode roundMode>
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id Cast__pT
 * @archs 3510 2201 2002 x90 9030
 * @brief 对标量的数据类型进行转换。
 */
__aicore__ inline U Cast(T valueIn);

// ScalarCast has been updated, please use Cast instead.
template <typename T, typename U, RoundMode roundMode>
__aicore__ inline U ScalarCast(T valueIn);

#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || \
     (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
     (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
// 所有架构共用的：float -> bfloat16_t
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id Cast__RCf32
 * @archs 3510 2201
 * @brief float类型标量数据转换成bfloat16\_t类型标量数据。
 */
__aicore__ inline bfloat16_t Cast(const float& fVal);
#endif

#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
// 各架构特化的：类型 -> float
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <typename T, typename U, typename>
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id Cast__pT
 * @archs 3510
 * @brief 该接口将输入数据转换为float类型。
 */
__aicore__ constexpr inline U Cast(T bVal);
#else
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id Cast__RCbf16
 * @archs 2201
 * @brief 该接口将输入数据转换为float类型。
 */
__aicore__ inline float Cast(const bfloat16_t& bVal);
#endif

#elif defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
                                (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
template <typename T>
__aicore__ constexpr inline float Cast(const T& bVal);
#endif

template <typename T>
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id WriteGmByPassDCache__PGpT_pT
 * @archs 3510 2201
 * @brief 不经过DCache向GM地址上写数据。
 * @param [in] addr 目标GM地址。
 * @param [in] value 待写入目标数据。
 */
__aicore__ inline void WriteGmByPassDCache(__gm__ T* addr, T value);

template <typename T>
/**
 * @ingroup simd_api.basic_api.scalar_compute
 * @override_id ReadGmByPassDCache__PGpT
 * @archs 3510 2201
 * @brief 不经过DCache从GM地址上读数据。
 * @param [in] addr 源GM地址。
 */
__aicore__ inline T ReadGmByPassDCache(__gm__ T* addr);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_scalar_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_SCALAR_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SCALAR_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SCALAR_INTF_H__
#endif
