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
 * \file kernel_operator_common_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_common_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COMMON_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_COMMON_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_COMMON_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "kernel_struct_mm.h"
#include "kernel_base_types.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#include "stub_fun.h"
#endif

/*
 * ingroup：SetAtomicAdd
 * brief：Set the next data from UB to the outside of AI Core whether the move write Tensor operation performs
 * atomic accumulation.
 */
namespace AscendC {
/*
 * @ingroup：SetNextTaskStart, WaitPreTaskEnd
 * @brief：In SuperKernel fusion mode, set wait flag between two operators
 */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ != 1001) && (__NPU_ARCH__ != 2002)
template <pipe_t AIV_PIPE = PIPE_MTE3, pipe_t AIC_PIPE = PIPE_FIX, bool FORCE = false>
#else
template <pipe_t AIV_PIPE = PIPE_MTE3, pipe_t AIC_PIPE = PIPE_MTE3, bool FORCE = false>
#endif
/**
 * @ingroup simd_api.basic_api.sync_control.inter_task_sync
 * @override_id SetNextTaskStart__z
 * @archs 3510 2201 3002 x90 9030 2002 1001
 * @brief 算子层面支持SuperKernel特性Early-Start能力的接口，调用后在TorchAir层面：1.GE图模式下进行默认启用; 2.npugraph_ex后端通过early_start选项进行控制启用。
 */
__aicore__ inline void SetNextTaskStart();

template <bool FORCE = false>
/**
 * @ingroup simd_api.basic_api.sync_control.inter_task_sync
 * @override_id WaitPreTaskEnd__z
 * @archs 3510 2201 3002 2002 1001 x90 9030
 * @brief 算子层面支持SuperKernel特性Early-Start能力的接口，调用后在TorchAir层面：1.GE图模式下进行默认启用; 2.npugraph_ex后端通过early_start选项进行控制启用。
 */
__aicore__ inline void WaitPreTaskEnd();

/**
 * @ingroup simd_api.basic_api.tool_interface.system_init
 * @override_id InitSocState__z
 * @archs 3510 2201 2002
 * @brief 本接口对AI Core的全局状态进行初始化，包括AIC（Cube Core）与AIV（Vector Core）的公共状态及各自特有状态。由于不同产品的实现存在差异，实际执行的初始化项也有所不同。
 */
__aicore__ inline void InitSocState();

// NOTICE: InitOutput has been deprecated and will be removed in the next version. Please use Fill instead!
template <typename T>
__aicore__ inline __in_pipe__(V)
    __out_pipe__(MTE3) void InitOutput(GlobalTensor<T> gmWorkspaceAddr, uint32_t size, T value = 0);

enum class AtomicDtype { ATOMIC_NONE = 0, ATOMIC_F32, ATOMIC_F16, ATOMIC_S16, ATOMIC_S32, ATOMIC_S8, ATOMIC_BF16 };

enum class AtomicOp { ATOMIC_SUM = 0 };

template <AtomicDtype type, AtomicOp op>
/**
 * @ingroup simd_api.basic_api.atomic_operations
 * @override_id SetStoreAtomicConfig__z
 * @archs 3510 2201 3002
 * @brief 设置原子操作开启位与原子操作类型。
 */
__aicore__ inline void SetStoreAtomicConfig();

/**
 * @ingroup simd_api.basic_api.atomic_operations
 * @override_id GetStoreAtomicConfig__Ru16_Ru16
 * @archs 3510 2201 3002
 * @brief 获取原子操作开启位与原子操作类型的值，详细说明见[表1](SetStoreAtomicConfig_ISASI.md#table622mcpsimp)。
 * @param [out] atomicType 原子操作开启位。0：无原子操作1：开启原子操作，进行原子操作的数据类型为float2：开启原子操作，进行原子操作的数据类型为half3：开启原子操作，进行原子操作的数据类型为int16_t4：开启原子操作，进行原子操作的数据类型为int32_t5：开启原子操作，进行原子操作的数据类型为int8_t6：开启原子操作，进行原子操作的数据类型为bfloat16_t
 * @param [out] atomicOp 原子操作类型。0：求和操作
 */
__aicore__ inline void GetStoreAtomicConfig(uint16_t& atomicType, uint16_t& atomicOp);

__aicore__ inline void CheckLocalMemoryIA(const CheckLocalMemoryIAParam& checkParams);

template <int8_t startBit, int8_t endBit>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id SetCtrlSpr__i64
 * @archs 3510 2201
 * @brief 对CTRL寄存器（控制寄存器）的特定比特位进行设置。通过模板参数startBit和endBit指定目标比特位范围，将运行参数value（超出比特位范围会出现编译报错）写入CTRL寄存器对应位置。
 * @param [in] value 起止比特位上新设置的值。
 */
__aicore__ static inline void SetCtrlSpr(int64_t value);

template <int8_t startBit, int8_t endBit>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id GetCtrlSpr__z
 * @archs 3510 2201
 * @brief 读取CTRL寄存器（控制寄存器）特定比特位上的值。
 */
__aicore__ static inline int64_t GetCtrlSpr();

template <int8_t startBit, int8_t endBit>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id ResetCtrlSpr__z
 * @archs 3510
 * @brief 对CTRL寄存器（控制寄存器）的特定比特位做重置。由模板参数\[startBit, endBit\]指定的比特位重置为默认值，默认值参考[表 常用CTRL寄存器比特位说明](SetCtrlSpr_ISASI.md#tab-950)。
 */
__aicore__ static inline void ResetCtrlSpr();

template <SaturationMode mode>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id SetSaturationFlag__b
 * @archs 3510 2201
 * @brief 设置不同数据类型的计算或精度转换时饱和模式的开启状态。
 * @param [in] enableSat 是否开启`mode`指定的饱和模式。`true`表示开启，`false`表示关闭。该参数无默认值，调用本接口时必须显式传入。
 */
__aicore__ inline void SetSaturationFlag(bool enableSat);

template <SaturationMode mode>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id GetSaturationFlag__z
 * @archs 3510 2201
 * @brief 获取`mode`指定的全局饱和模式的开启状态，该状态通过[SetSaturationFlag](SetSaturationFlag_ISASI.md)设置。饱和模式用于处理计算或精度转换过程中结果溢出或出现INF/NAN的场景，详细说明请参考[SetSaturationFlag功能说明](SetSaturationFlag_ISASI.md#功能说明)。
 */
__aicore__ inline bool GetSaturationFlag();

template <OverrideStrategy strategy>
/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id SetSaturationStrategy__z
 * @archs 3510
 * @brief 设置精度转换接口使用的饱和模式配置策略。
 */
__aicore__ inline void SetSaturationStrategy();

/**
 * @ingroup simd_api.basic_api.special_register_access
 * @override_id GetSaturationStrategy__z
 * @archs 3510
 * @brief 获取精度转换接口当前使用的饱和模式配置策略。饱和模式用于处理精度转换过程中结果溢出或出现INF/NAN的场景，详细说明请参考[SetSaturationFlag功能说明](SetSaturationFlag_ISASI.md#功能说明)。
 */
__aicore__ inline OverrideStrategy GetSaturationStrategy();

template <CacheRwMode rwMode, CacheMode cacheMode>
__aicore__ inline void SetScalarCacheMode();

template <CacheRwMode rwMode>
__aicore__ inline CacheMode GetScalarCacheMode();
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_common_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_COMMON_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COMMON_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COMMON_INTF_H__
#endif
