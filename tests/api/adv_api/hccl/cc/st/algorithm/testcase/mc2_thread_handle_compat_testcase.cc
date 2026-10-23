/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <cstddef>
#include <cstdint>
#include "gtest/gtest.h"
#include "mc2_thread_handle_compat.h"

namespace {
using mc2_thread_compat::ThreadEntityV0;
using mc2_thread_compat::UnwrapThreadHandle;

ThreadEntityV0 MakeEntity(ThreadHandle rawHandle)
{
    ThreadEntityV0 entity{};
    entity.abiHeader = {
        mc2_thread_compat::THREAD_ENTITY_VERSION, mc2_thread_compat::THREAD_ENTITY_MAGIC_WORD, sizeof(entity), 0};
    entity.compatThreadHandle = rawHandle;
    return entity;
}

TEST(Mc2ThreadHandleCompat, LegacyHandleIsPreserved)
{
    // 仅提供一个指针大小的存储，旧句柄不能按完整 Entity 读取。
    uint64_t legacyObject = 0;
    const ThreadHandle handle = reinterpret_cast<ThreadHandle>(&legacyObject);
    ThreadHandle rawHandle = 0;
    EXPECT_EQ(UnwrapThreadHandle(handle, rawHandle), HCCL_SUCCESS);
    EXPECT_EQ(rawHandle, handle);
}

TEST(Mc2ThreadHandleCompat, EntityResolvesWithoutChangingPublishedHandle)
{
    uint64_t legacyObject = 0;
    const ThreadHandle original = reinterpret_cast<ThreadHandle>(&legacyObject);
    auto entity = MakeEntity(original);
    const ThreadHandle published = reinterpret_cast<ThreadHandle>(&entity);
    ThreadHandle rawHandle = 0;
    EXPECT_EQ(UnwrapThreadHandle(published, rawHandle), HCCL_SUCCESS);
    EXPECT_EQ(rawHandle, original);
    EXPECT_EQ(published, reinterpret_cast<ThreadHandle>(&entity));
    EXPECT_EQ(entity.compatThreadHandle, original);
}

TEST(Mc2ThreadHandleCompat, NullHandleClearsOutput)
{
    ThreadHandle rawHandle = 1;
    EXPECT_EQ(UnwrapThreadHandle(0, rawHandle), HCCL_E_PTR);
    EXPECT_EQ(rawHandle, 0U);
}

TEST(Mc2ThreadHandleCompat, UnsupportedVersionDoesNotFallBackToLegacy)
{
    auto entity = MakeEntity(0x1000U);
    ++entity.abiHeader.version;
    ThreadHandle rawHandle = 1;
    EXPECT_EQ(UnwrapThreadHandle(reinterpret_cast<ThreadHandle>(&entity), rawHandle), HCCL_E_PARA);
    EXPECT_EQ(rawHandle, 0U);
}

TEST(Mc2ThreadHandleCompat, ShortEntityIsRejectedBeforeReadingCompatibilityField)
{
    CommAbiHeader header{
        mc2_thread_compat::THREAD_ENTITY_VERSION, mc2_thread_compat::THREAD_ENTITY_MAGIC_WORD, sizeof(CommAbiHeader),
        0};
    ThreadHandle rawHandle = 1;
    EXPECT_EQ(UnwrapThreadHandle(reinterpret_cast<ThreadHandle>(&header), rawHandle), HCCL_E_PARA);
    EXPECT_EQ(rawHandle, 0U);
}

TEST(Mc2ThreadHandleCompat, UninitializedEntityIsRejected)
{
    auto entity = MakeEntity(0);
    ThreadHandle rawHandle = 1;
    EXPECT_EQ(UnwrapThreadHandle(reinterpret_cast<ThreadHandle>(&entity), rawHandle), HCCL_E_INTERNAL);
    EXPECT_EQ(rawHandle, 0U);
}

TEST(Mc2ThreadHandleCompat, SelfReferencingEntityIsRejected)
{
    auto entity = MakeEntity(0);
    entity.compatThreadHandle = reinterpret_cast<ThreadHandle>(&entity);
    ThreadHandle rawHandle = 1;
    EXPECT_EQ(UnwrapThreadHandle(entity.compatThreadHandle, rawHandle), HCCL_E_INTERNAL);
    EXPECT_EQ(rawHandle, 0U);
}
} // namespace
