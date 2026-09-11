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
 * \file kernel_operator_determine_compute_sync_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_determine_compute_sync_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H
#define ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
/*
 * @ingroup WaitPreBlock
 * @brief wait previous compute finish
 * @param [in] global memory workspace
 */
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id InitDetermineComputeWorkspace__Rgtg1x3_i32_Rltg1x3_i32
 * @archs 3510 2201 2002
 * @brief InitDetermineComputeWorkspace是基于核间顺序执行的确定性计算的初始化配置接口，能够初始化GM共享内存的值，完成初始化后才可以调用[WaitPreBlock](WaitPreBlock.md)和[NotifyNextBlock](NotifyNextBlock.md)。以上三个接口共同完成基于核间顺序执行的确定性计算，确定性计算的具体含义请参考[确定性计算](../../atomic_operations/key_features.md)。
 */
__aicore__ inline void InitDetermineComputeWorkspace(
    GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);
/*
 * @ingroup WaitPreBlock
 * @brief wait previous compute finish
 * @param [in] global memory workspace
 */
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id WaitPreBlock__Rgtg1x3_i32_Rltg1x3_i32
 * @archs 3510 2201 2002
 * @brief WaitPreBlock和NotifyNextBlock是核间同步控制接口。NotifyNextBlock通过写全局内存来通知其他核当前核已执行完成，其他核可以继续往下执行；WaitPreBlock通过读取全局内存，判断当前核是否可以继续往下执行。
 */
__aicore__ inline void WaitPreBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);

/*
 * @ingroup NotifyNextBlock
 * @brief wait previous compute finish
 * @param [in] global memory workspace
 */
/**
 * @ingroup simd_api.basic_api.sync_control.inter_core_sync
 * @override_id NotifyNextBlock__Rgtg1x3_i32_Rltg1x3_i32
 * @archs 3510 2201 2002
 * @brief WaitPreBlock和NotifyNextBlock是核间同步控制接口。NotifyNextBlock通过写全局内存来通知其他核当前核已执行完成，其他核可以继续往下执行；WaitPreBlock通过读取全局内存，判断当前核是否可以继续往下执行。
 */
__aicore__ inline void NotifyNextBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_determine_compute_sync_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H__
#endif
