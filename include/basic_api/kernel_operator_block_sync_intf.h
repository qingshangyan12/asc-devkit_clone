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
 * \file kernel_operator_block_sync_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_block_sync_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_BLOCK_SYNC_INTF_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_BLOCK_SYNC_INTF_H
#define ASCENDC_MODULE_OPERATOR_BLOCK_SYNC_INTF_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/kernel_event.h"
#include "../../impl/basic_api/kernel_reg.h"
#include "kernel_tensor.h"
#include "../../impl/basic_api/kernel_tensor_base.h"
#include "../../impl/basic_api/utils/kernel_utils_mode.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#include "stub_fun.h"
#endif

namespace AscendC {

template <HardEvent event>
/**
 * @ingroup simd_api.basic_api.sync_control.intra_core_sync
 * @override_id SetFlag__i32
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief 如图1所示。
 * @param [in] eventID 事件ID。数据类型为int32_t类型。eventID的取值范围与产品型号有关，具体请参考[约束说明](#section633mcpsimp)。
 */
__aicore__ inline void SetFlag(int32_t eventID);

template <HardEvent event>
/**
 * @ingroup simd_api.basic_api.sync_control.intra_core_sync
 * @override_id WaitFlag__i32
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief 如图1所示。
 * @param [in] eventID 事件ID。数据类型为int32_t类型。eventID的取值范围与产品型号有关，具体请参考[约束说明](#section633mcpsimp)。
 */
__aicore__ inline void WaitFlag(int32_t eventID);

template <pipe_t pipe>
/**
 * @ingroup simd_api.basic_api.sync_control.intra_core_sync
 * @override_id PipeBarrier__z
 * @archs 3510 2201 3002 2002 1001 x90 9030
 * @brief 如下图1所示，完成同一流水线内的同步控制，用于在同一流水线内部约束执行顺序。其作用是，保证前序指令中所有数据的读写工作全部完成，后序指令才能执行。
 */
__aicore__ inline void PipeBarrier();

template <MemDsbT arg0>
/**
 * @ingroup simd_api.basic_api.sync_control.intra_core_sync
 * @override_id DataSyncBarrier__z
 * @archs 3510 2201 3002
 * @brief 阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可通过参数控制）执行结束，用于解决Scalar的内存依赖。
 */
__aicore__ inline void DataSyncBarrier();

/*
 * @ingroup：IBSet, IBWait
 * @brief：Set the flag bit of a core
 * @param [in] gmWorkspace GlobalTensor to store core state
 * @param [in] ubWorkspce LocalTensor for current core
 * @param [in] blockIdx the idx number waiting for the core
 * @param [in] eventID Set and wait events
 */
template <bool isAIVOnly = true>
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id IBSet__RCgtg1x3_i32_RCltg1x3_i32_i32_i32
 * @archs 3510 2201 2002 1001
 * @brief 当不同核之间操作同一块全局内存且可能存在读后写、写后读以及写后写等数据依赖问题时，通过调用该函数来插入同步语句来避免上述数据依赖时可能出现的数据读写错误问题。
 * @param [in] gmWorkspace 外部存储核状态的公共缓存，类型为GlobalTensor。GlobalTensor数据结构的定义请参考[GlobalTensor](../../data_structures/GlobalTensor/GlobalTensor.md)。
 * @param [in] ubWorkspace 存储当前核状态的公共缓存。类型为[LocalTensor](../../data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] blockIdx 表示等待核的idx号，取值范围：[0,核数-1]。
 * @param [in] eventID 用来控制当前核的set、wait事件。
 */
__aicore__ inline void IBSet(
    const GlobalTensor<int32_t>& gmWorkspace, const LocalTensor<int32_t>& ubWorkspace, int32_t blockIdx,
    int32_t eventID);

template <bool isAIVOnly = true>
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id IBWait__RCgtg1x3_i32_RCltg1x3_i32_i32_i32
 * @archs 3510 2201 2002 1001
 * @brief 当不同核之间操作同一块全局内存且可能存在读后写、写后读以及写后写等数据依赖问题时，通过调用该函数来插入同步语句来避免上述数据依赖时可能出现的数据读写错误问题。
 * @param [in] gmWorkspace 外部存储核状态的公共缓存，类型为GlobalTensor。GlobalTensor数据结构的定义请参考[GlobalTensor](../../data_structures/GlobalTensor/GlobalTensor.md)。
 * @param [in] ubWorkspace 存储当前核状态的公共缓存。类型为[LocalTensor](../../data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] blockIdx 表示等待核的idx号，取值范围：[0,核数-1]。
 * @param [in] eventID 用来控制当前核的set、wait事件。
 */
__aicore__ inline void IBWait(
    const GlobalTensor<int32_t>& gmWorkspace, const LocalTensor<int32_t>& ubWorkspace, int32_t blockIdx,
    int32_t eventID);
/*
 * @ingroup：SyncALL
 * @brief：Set flag bits of all cores
 * @param [in] gmWorkspace GlobalTensor to store core state
 * @param [in] ubWorkspce LocalTensor for current core
 */
template <bool isAIVOnly = true>
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id SyncAll__RCgtg1x3_i32_RCltg1x3_i32_i32
 * @archs 3510 2201 2002 1001 x90 9030
 * @brief SyncAll是核间同步控制接口。
 */
__aicore__ inline void SyncAll(
    const GlobalTensor<int32_t>& gmWorkspace, const LocalTensor<int32_t>& ubWorkspace, const int32_t usedCores = 0);

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
template <bool isAIVOnly = true, const SyncAllConfig& config = DEFAULT_SYNC_ALL_CONFIG>
__aicore__ inline void SyncAll();
#else
template <bool isAIVOnly = true>
__aicore__ inline void SyncAll();
#endif

template <uint8_t modeId, pipe_t pipe>
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id CrossCoreSetFlag__u16
 * @archs 3510 2201
 * @brief 在核间同步场景中，CrossCoreSetFlag接口和[CrossCoreWaitFlag](CrossCoreWaitFlag_ISASI.md)接口配对工作，使用时需传入核间同步的标记ID（flagId），每个ID对应一个用于控制同步的计数器。
 * @param [in] flagId 核间同步的标记。不同产品对flagId的取值范围说明请参见[flagId取值范围说明](#flagId取值范围说明)。
 */
__aicore__ inline void CrossCoreSetFlag(uint16_t flagId);

template <uint8_t modeId = 0, pipe_t pipe = PIPE_S>
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id CrossCoreWaitFlag__u16
 * @archs 3510 2201
 * @brief 在核间同步场景中，CrossCoreSetFlag接口和CrossCoreWaitFlag接口配对工作，具体功能请参考[CrossCoreSetFlag功能说明](CrossCoreSetFlag_ISASI.md#section618mcpsimp)。
 * @param [in] flagId 核间同步的标记。不同产品对flagId的取值范围说明请参见[flagId取值范围说明](#flagId取值范围说明)。
 */
__aicore__ inline void CrossCoreWaitFlag(uint16_t flagId);

template <pipe_t src, pipe_t dst>
class TQueSync {
public:
    __aicore__ inline void SetFlag(TEventID id);
    __aicore__ inline void WaitFlag(TEventID id);
};

} // namespace AscendC
#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_block_sync_intf_impl.h"
#endif
#endif // KERNEL_BLOCK_SYNC_INTF_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_BLOCK_SYNC_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_BLOCK_SYNC_INTF_H__
#endif
