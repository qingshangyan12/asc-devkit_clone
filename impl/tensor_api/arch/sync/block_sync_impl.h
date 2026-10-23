/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/arch/sync/block_sync_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"tensor_api/arch/sync/block_sync.h\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file block_sync_impl.h
 * \brief Synchronization implementation.
 */
#ifndef IMPL_TENSOR_API_ARCH_SYNC_BLOCK_SYNC_IMPL_H
#define IMPL_TENSOR_API_ARCH_SYNC_BLOCK_SYNC_IMPL_H

#include "c_api/asc_simd.h"

namespace asc {
namespace te {

__aicore__ inline void sync_block_arrive(pipe_t pipe, int64_t flag_id) { asc_sync_block_arrive(pipe, flag_id); }

__aicore__ inline void sync_block_wait(pipe_t pipe, int64_t flag_id) { asc_sync_block_wait(pipe, flag_id); }

__aicore__ inline void sync_intra_arrive(pipe_t pipe, uint64_t sync_id) { asc_sync_intra_arrive(pipe, sync_id); }

__aicore__ inline void sync_intra_wait(pipe_t pipe, uint64_t sync_id) { asc_sync_intra_wait(pipe, sync_id); }

__aicore__ inline void sync_inter_arrive(pipe_t pipe, int64_t flag_id) { asc_sync_inter_arrive(pipe, flag_id); }

__aicore__ inline void sync_inter_wait(pipe_t pipe, int64_t flag_id) { asc_sync_inter_wait(pipe, flag_id); }

__aicore__ inline void sync_subblock_arrive(pipe_t pipe, int64_t flag_id) { asc_sync_subblock_arrive(pipe, flag_id); }

__aicore__ inline void sync_subblock_wait(pipe_t pipe, int64_t flag_id) { asc_sync_subblock_wait(pipe, flag_id); }

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_SYNC_BLOCK_SYNC_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
