/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_H
#endif

/*!
 * \file reg_sync_control.h
 * \brief Register memory synchronization interfaces.
 */
#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_SYNC_CONTROL_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_SYNC_CONTROL_H

#include "tensor_api/experimental/utils/reg_enum.h"

namespace asc {
namespace te {
namespace experimental {
/**
 * @ingroup simd_api.tensor_api.reg_vector_compute.reg_sync_control
 * @brief Synchronizes UB accesses between two pipelines in a SIMD VF.
 *
 * @override_id te_experimental_mem_bar__void__template_src_dst
 * @archs 3510
 * @tparam src Source pipeline whose preceding UB accesses must complete.
 * @tparam dst Destination pipeline whose subsequent UB accesses wait for the source pipeline.
 * @note Only the source/destination combinations documented for this interface are supported. This interface does not
 * synchronize MTE pipelines or different NPU cores.
 */
template <mem_type src, mem_type dst>
__simd_callee__ inline void mem_bar();
} // namespace experimental
} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/arch/vector/reg_sync_control_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_REG_SYNC_CONTROL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_SYNC_CONTROL_H
#endif
