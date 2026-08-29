/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <type_traits>
#include <gtest/gtest.h>
#include "c_api/stub/cce_stub.h"
#include "include/c_api/asc_simd.h"

using SyncFunction = void (*)(pipe_t, uint16_t);
using DeprecatedSyncFunction = void (*)(pipe_t, int64_t);

namespace {
constexpr int64_t kConstexprDeprecatedFlagId = 65537;
}

static_assert(std::is_same_v<decltype(static_cast<SyncFunction>(&asc_sync_block_arrive)), SyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<SyncFunction>(&asc_sync_block_wait)), SyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<SyncFunction>(&asc_sync_subblock_arrive)), SyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<SyncFunction>(&asc_sync_subblock_wait)), SyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<SyncFunction>(&asc_sync_inter_arrive)), SyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<SyncFunction>(&asc_sync_inter_wait)), SyncFunction>);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
static_assert(std::is_same_v<decltype(static_cast<DeprecatedSyncFunction>(&asc_sync_block_arrive)), DeprecatedSyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<DeprecatedSyncFunction>(&asc_sync_block_wait)), DeprecatedSyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<DeprecatedSyncFunction>(&asc_sync_subblock_arrive)), DeprecatedSyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<DeprecatedSyncFunction>(&asc_sync_subblock_wait)), DeprecatedSyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<DeprecatedSyncFunction>(&asc_sync_inter_arrive)), DeprecatedSyncFunction>);
static_assert(std::is_same_v<decltype(static_cast<DeprecatedSyncFunction>(&asc_sync_inter_wait)), DeprecatedSyncFunction>);
#pragma GCC diagnostic pop

[[maybe_unused]] void SyncFlagIdUint16UsesCanonicalOverload(pipe_t pipe)
{
    constexpr uint16_t flag_id = 1;
    asc_sync_block_arrive(pipe, flag_id);
    asc_sync_block_wait(pipe, flag_id);
    asc_sync_subblock_arrive(pipe, flag_id);
    asc_sync_subblock_wait(pipe, flag_id);
    asc_sync_inter_arrive(pipe, flag_id);
    asc_sync_inter_wait(pipe, flag_id);
}

[[maybe_unused]] void SyncFlagIdConstexprUsesDeprecatedOverload(pipe_t pipe)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    asc_sync_block_arrive(pipe, kConstexprDeprecatedFlagId);
    asc_sync_block_wait(pipe, kConstexprDeprecatedFlagId);
    asc_sync_subblock_arrive(pipe, kConstexprDeprecatedFlagId);
    asc_sync_subblock_wait(pipe, kConstexprDeprecatedFlagId);
    asc_sync_inter_arrive(pipe, kConstexprDeprecatedFlagId);
    asc_sync_inter_wait(pipe, kConstexprDeprecatedFlagId);
#pragma GCC diagnostic pop
}
