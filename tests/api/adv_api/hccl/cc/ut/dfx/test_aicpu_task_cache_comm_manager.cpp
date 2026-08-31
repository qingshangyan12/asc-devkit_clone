/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 *
 * Unit tests for AicpuTaskCacheCommManager (ported from hccl commit e43eeb52 — A5 aicpu task cache).
 * Tests singleton identity, AddCommTagMap, EvictTaskCache (dlsym not loaded → internal map clear only).
 */
#include "aicpu_task_cache_comm_manager.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

// Stubs for symbols not available in standalone test env (no libhcomm loaded, no libsecurec).
extern "C" bool HcommIsSupportHcommAicpuTsTaskCacheClear(void) { return false; }

bool HcclCheckLogLevel(int, int) { return false; }
bool IsErrorToWarn() { return false; }

extern "C" errno_t memset_s(void* s, size_t smax, int c, size_t n)
{
    if (s == nullptr || n > smax) {
        return EINVAL;
    }
    std::memset(s, c, n);
    return 0;
}

using namespace ops_hccl;

static int32_t g_pass = 0;
static int32_t g_fail = 0;

#define RUN_TEST(fn)                                                                                                  \
    do {                                                                                                               \
        int32_t before = g_fail;                                                                                     \
        fn();                                                                                                          \
        printf("[%s] %s\n", (g_fail == before) ? "PASS" : "FAIL", #fn);                                              \
        if (g_fail == before) { ++g_pass; }                                                                          \
    } while (0)

#define EXPECT_TRUE(c)                                                                                               \
    do {                                                                                                               \
        if (!(c)) {                                                                                                  \
            printf("  expect-true FAILED: %s @%d\n", #c, __LINE__);                                                  \
            ++g_fail;                                                                                                \
        }                                                                                                             \
    } while (0)

#define EXPECT_EQ(a, b)                                                                                              \
    do {                                                                                                              \
        if ((a) != (b)) {                                                                                            \
            printf("  expect-eq FAILED: %s != %s @%d\n", #a, #b, __LINE__);                                          \
            ++g_fail;                                                                                                \
        }                                                                                                             \
    } while (0)

static void TestSingletonIdentity()
{
    auto& a = AicpuTaskCacheCommManager::Instance();
    auto& b = AicpuTaskCacheCommManager::Instance();
    EXPECT_TRUE(&a == &b);
}

static void TestAddAndEvict()
{
    auto& mgr = AicpuTaskCacheCommManager::Instance();
    HcclComm comm1 = reinterpret_cast<HcclComm>(0x1000);
    HcclComm comm2 = reinterpret_cast<HcclComm>(0x2000);

    mgr.AddCommTagMap(comm1, "cache_tag_a");
    mgr.AddCommTagMap(comm1, "cache_tag_b");
    mgr.AddCommTagMap(comm2, "cache_tag_c");

    HcclResult ret = mgr.EvictTaskCache(comm1);
    EXPECT_EQ(ret, HCCL_SUCCESS);
}

static void TestEvictUnknownComm()
{
    auto& mgr = AicpuTaskCacheCommManager::Instance();
    HcclComm unknown = reinterpret_cast<HcclComm>(0xDEAD);
    HcclResult ret = mgr.EvictTaskCache(unknown);
    EXPECT_EQ(ret, HCCL_SUCCESS);
}

static void TestReEvictClearedComm()
{
    auto& mgr = AicpuTaskCacheCommManager::Instance();
    HcclComm comm = reinterpret_cast<HcclComm>(0x5000);
    mgr.AddCommTagMap(comm, "tag_x");
    EXPECT_EQ(mgr.EvictTaskCache(comm), HCCL_SUCCESS);
    EXPECT_EQ(mgr.EvictTaskCache(comm), HCCL_SUCCESS);
}

static void TestEvictAfterAdd()
{
    auto& mgr = AicpuTaskCacheCommManager::Instance();
    HcclComm comm = reinterpret_cast<HcclComm>(0x6000);
    mgr.AddCommTagMap(comm, "tag_y");
    EXPECT_EQ(mgr.EvictTaskCache(comm), HCCL_SUCCESS);
    mgr.AddCommTagMap(comm, "tag_z");
    EXPECT_EQ(mgr.EvictTaskCache(comm), HCCL_SUCCESS);
}

int main()
{
    printf("=== AicpuTaskCacheCommManager tests (from hccl e43eeb52) ===\n");
    RUN_TEST(TestSingletonIdentity);
    RUN_TEST(TestAddAndEvict);
    RUN_TEST(TestEvictUnknownComm);
    RUN_TEST(TestReEvictClearedComm);
    RUN_TEST(TestEvictAfterAdd);
    printf("\n=== Results: %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
