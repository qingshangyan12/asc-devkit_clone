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

#include "../../impl/basic_api/utils/kernel_utils_ptr_overload.h"
namespace AscendC {
/*
 * @ingroup WaitPreBlock
 * @brief wait previous compute finish
 * @param [in] global memory workspace
 */
__aicore__ inline void InitDetermineComputeWorkspace(
    GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);
/*
 * @ingroup WaitPreBlock
 * @brief wait previous compute finish
 * @param [in] global memory workspace
 */
__aicore__ inline void WaitPreBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);

/*
 * @ingroup NotifyNextBlock
 * @brief wait previous compute finish
 * @param [in] global memory workspace
 */
__aicore__ inline void NotifyNextBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);
/*
 * @ingroup InitDetermineComputeWorkspace, WaitPreBlock, NotifyNextBlock
 * @brief Initialize, wait for, and notify the previous compute stage
 * @param [in] gmWorkspace raw GM pointer to the synchronization workspace
 * @param [in] ubWorkspace raw UB pointer to the current core's workspace
 *
 * These three are NOT a forwarding layer, for the same reason as IBSet / IBWait: their Tensor bodies
 * drive the workspace through LocalTensor / GlobalTensor *operations* (`GetValue`, `SetValue`,
 * `DataCopy`, `ubWorkspace[offset]`), which a raw pointer does not carry.  The pointer overloads
 * re-express the same body on the pointer-level primitives those operations reach; the
 * operation-by-operation mapping and the equivalence argument are in the definitions in
 * impl/basic_api/kernel_operator_determine_compute_sync_intf_impl.h.
 *
 * The three Tensor declarations above take their workspaces by non-const reference; the pointer
 * forms take the operands by value (a pointer is a value) and are constrained to at least one raw
 * pointer operand, so no Tensor call site can become ambiguous.
 */
template <
    typename GmPtr, typename UbPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<GmPtr, UbPtr>::value, bool>::type = true>
__aicore__ inline void InitDetermineComputeWorkspace(GmPtr gmWorkspace, UbPtr ubWorkspace);

template <
    typename GmPtr, typename UbPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<GmPtr, UbPtr>::value, bool>::type = true>
__aicore__ inline void WaitPreBlock(GmPtr gmWorkspace, UbPtr ubWorkspace);

template <
    typename GmPtr, typename UbPtr,
    typename Std::enable_if<CubePtrOverload::AnyPtr<GmPtr, UbPtr>::value, bool>::type = true>
__aicore__ inline void NotifyNextBlock(GmPtr gmWorkspace, UbPtr ubWorkspace);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_determine_compute_sync_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_H__
#endif
