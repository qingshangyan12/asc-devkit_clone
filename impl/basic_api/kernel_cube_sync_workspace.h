/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_CUBE_SYNC_WORKSPACE_H
#define ASCENDC_CUBE_SYNC_WORKSPACE_H
#include "kernel_cube_pointer_traits.h"
#if !defined(ASCENDC_CPU_DEBUG)
namespace AscendC {
namespace Impl {
struct CubeSyncGlobalWorkspace {
    __gm__ int32_t* address;
    uint8_t cacheMode;
    __aicore__ inline CubeSyncGlobalWorkspace operator[](uint32_t offset) const
    {
        return {address + offset, cacheMode};
    }
    __aicore__ inline __gm__ int32_t* GetPhyAddr() const { return address; }
};
struct CubeSyncLocalWorkspace {
    __ubuf__ int32_t* address;
    __aicore__ inline CubeSyncLocalWorkspace operator[](uint32_t offset) const { return {address + offset}; }
    __aicore__ inline __ubuf__ int32_t* GetPhyAddr() const { return address; }
    __aicore__ inline int32_t GetValue(uint32_t offset) const { return address[offset]; }
    __aicore__ inline void SetValue(uint32_t offset, int32_t value) const { address[offset] = value; }
};
template <typename G>
__aicore__ inline CubeSyncGlobalWorkspace CubeSyncGlobal(const G& operand)
{
    static_assert(Std::is_same<CubeElement<G>, int32_t>::value, "Sync workspace must contain int32_t");
    CheckCubeSpace<Hardware::GM>(operand);
#if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
    return {(__gm__ int32_t*)GetUnderlyingPtr(operand), ExtractCacheMode(operand)};
#else
    return {(__gm__ int32_t*)GetUnderlyingPtr(operand), 0};
#endif
}
template <typename U>
__aicore__ inline CubeSyncLocalWorkspace CubeSyncLocal(const U& operand)
{
    static_assert(Std::is_same<CubeElement<U>, int32_t>::value, "Sync workspace must contain int32_t");
    CheckCubeSpace<Hardware::UB>(operand);
    return {(__ubuf__ int32_t*)GetUnderlyingPtr(operand)};
}
__aicore__ inline void DataCopy(const CubeSyncLocalWorkspace& dst, const CubeSyncGlobalWorkspace& src,
    uint32_t count)
{
    const DataCopyParams params(1, count * sizeof(int32_t) / ONE_BLK_SIZE, 0, 0);
#if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
    DataCopyGM2UBImpl(dst.address, src.address, params, src.cacheMode);
#else
    DataCopyGM2UBImpl(dst.address, src.address, params);
#endif
}
__aicore__ inline void DataCopy(const CubeSyncGlobalWorkspace& dst, const CubeSyncLocalWorkspace& src,
    uint32_t count)
{
    const DataCopyParams params(1, count * sizeof(int32_t) / ONE_BLK_SIZE, 0, 0);
#if __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102
    DataCopyUB2GMImpl(dst.address, src.address, params, dst.cacheMode);
#else
    DataCopyUB2GMImpl(dst.address, src.address, params);
#endif
}
} // namespace Impl
} // namespace AscendC
#endif
#endif
