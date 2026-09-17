/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_BLOCK_SYNC_POINTER_IMPL_H
#define ASCENDC_BLOCK_SYNC_POINTER_IMPL_H
#include "kernel_cube_pointer_traits.h"
#include "kernel_cube_sync_workspace.h"
namespace AscendC {
#if !defined(ASCENDC_CPU_DEBUG)
template <bool isAIVOnly = true, typename G, typename U,
    typename Std::enable_if<Impl::CubeHasPointer<G, U>::value, bool>::type = true>
__aicore__ inline void IBSet(const G& gmWorkspace, const U& ubWorkspace, int32_t blockIdx, int32_t eventID)
{
    IBSetWorkspaceImpl<isAIVOnly>(Impl::CubeSyncGlobal(gmWorkspace), Impl::CubeSyncLocal(ubWorkspace),
        blockIdx, eventID);
}
template <bool isAIVOnly = true, typename G, typename U,
    typename Std::enable_if<Impl::CubeHasPointer<G, U>::value, bool>::type = true>
__aicore__ inline void IBWait(const G& gmWorkspace, const U& ubWorkspace, int32_t blockIdx, int32_t eventID)
{
    IBWaitWorkspaceImpl<isAIVOnly>(Impl::CubeSyncGlobal(gmWorkspace), Impl::CubeSyncLocal(ubWorkspace),
        blockIdx, eventID);
}
template <bool isAIVOnly = true, typename G, typename U,
    typename Std::enable_if<Impl::CubeHasPointer<G, U>::value, bool>::type = true>
__aicore__ inline void SyncAll(const G& gmWorkspace, const U& ubWorkspace, const int32_t usedCores = 0)
{
    static_assert(Std::is_same<Impl::CubeElement<G>, int32_t>::value &&
        Std::is_same<Impl::CubeElement<U>, int32_t>::value, "SyncAll workspaces must contain int32_t");
    Impl::CheckCubeSpace<Hardware::GM>(gmWorkspace);
    Impl::CheckCubeSpace<Hardware::UB>(ubWorkspace);
    SoftSyncAllImpl<isAIVOnly>((__gm__ int32_t*)Impl::GetUnderlyingPtr(gmWorkspace),
        (__ubuf__ int32_t*)Impl::GetUnderlyingPtr(ubWorkspace), usedCores);
}
#endif
} // namespace AscendC
#endif
