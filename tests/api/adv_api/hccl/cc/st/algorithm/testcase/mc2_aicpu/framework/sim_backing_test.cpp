/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include "sim_common.h"
#include "sim_npu.h"
#include "sim_world.h"

using namespace HcclSim;

// SimNpu 后备内存：AllocMemory 返回的虚拟地址必须能映射到可读写 host 内存，
// 供 TaskGraphExecutor 执行真实数据搬运。
TEST(AicpuStSimBacking, AllocMemoryReturnsUsableBacking)
{
    SimWorld::Global()->Deinit();
    SimWorld::Global()->Init(TopoMeta{{{{0, 1}}}}, DevType::DEV_TYPE_950);

    SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t addr = npu.AllocMemory(BufferType::INPUT, 64);
    u8* base = npu.GetBufferBase(BufferType::INPUT, 64);
    ASSERT_NE(base, nullptr);
    for (uint64_t i = 0; i < 64; ++i) {
        base[i] = static_cast<u8>(i);
    }

    DataSlice slice;
    ASSERT_EQ(npu.GetSlice(addr + 16, 8, slice), HcclResult::HCCL_SUCCESS);
    EXPECT_EQ(slice.GetOffset(), 16U);
    u8* resolved = npu.GetBufferBase(BufferType::INPUT, slice.GetOffset() + slice.GetSize()) + slice.GetOffset();
    EXPECT_EQ(resolved[0], 16);

    SimWorld::Global()->Deinit();
}

// CCL 块从未 AllocMemory，按需惰性生长，避免 200MB*rank 常驻开销。
TEST(AicpuStSimBacking, CclBackingGrowsLazily)
{
    SimWorld::Global()->Deinit();
    SimWorld::Global()->Init(TopoMeta{{{{0, 1}}}}, DevType::DEV_TYPE_950);

    SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(0);
    MemBlock ccl = npu.GetMemBlock(BufferType::CCL);
    u8* base = npu.GetBufferBase(BufferType::CCL, 32);
    ASSERT_NE(base, nullptr);
    base[31] = 0xAB;
    DataSlice slice;
    ASSERT_EQ(npu.GetSlice(ccl.startAddr + 31, 1, slice), HcclResult::HCCL_SUCCESS);
    EXPECT_EQ(npu.GetBufferBase(BufferType::CCL, 32)[31], 0xAB);

    SimWorld::Global()->Deinit();
}
