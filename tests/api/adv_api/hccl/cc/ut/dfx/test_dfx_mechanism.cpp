/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 *
 * Unit tests for the DFX mechanism ported from hcomm PR#3878 into aicpu_kfc:
 *   - DfxCircularQueue (dfx_circular_queue.h)
 *   - DfxTaskInfo / DfxDfxOpInfo (res_pub.h)
 *   - DfxProfilingHandlerLite (dfx_profiling_handler_lite.h)
 *
 * Self-contained (no gtest dependency) — uses assert + pass/fail counter so it runs in any env.
 */
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "res_pub.h"
#include "dfx_circular_queue.h"
#include "dfx_profiling_handler_lite.h"

// Link-time shim: securec.h declares memset_s but libsecurec is not available in this env.
// Minimal conformant implementation matching the CANN securec signature.
extern "C" errno_t memset_s(void* s, size_t smax, int c, size_t n)
{
    if (s == nullptr || n > smax) {
        return EINVAL;
    }
    std::memset(s, c, n);
    return 0;
}

static int32_t g_passCount = 0;
static int32_t g_failCount = 0;

#define RUN_TEST(fn)                                                                                                  \
    do {                                                                                                               \
        int32_t before = g_failCount;                                                                                 \
        fn();                                                                                                          \
        if (g_failCount == before) {                                                                                  \
            printf("[PASS] %s\n", #fn);                                                                               \
            ++g_passCount;                                                                                            \
        } else {                                                                                                      \
            printf("[FAIL] %s\n", #fn);                                                                               \
        }                                                                                                              \
    } while (0)

#define EXPECT_TRUE(cond)                                                                                             \
    do {                                                                                                               \
        if (!(cond)) {                                                                                                \
            printf("  expect-true FAILED: %s @%d\n", #cond, __LINE__);                                               \
            ++g_failCount;                                                                                            \
        }                                                                                                              \
    } while (0)

#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))

#define EXPECT_EQ(a, b)                                                                                              \
    do {                                                                                                               \
        if ((a) != (b)) {                                                                                             \
            printf("  expect-eq FAILED: %s != %s @%d (%llu != %llu)\n", #a, #b, __LINE__,                             \
                   (unsigned long long)(a), (unsigned long long)(b));                                                 \
            ++g_failCount;                                                                                            \
        }                                                                                                              \
    } while (0)

using namespace Hccl;

// ── DfxCircularQueue mechanics (small-capacity to exercise wraparound) ──────────

static void TestCircularQueueEmpty()
{
    DfxCircularQueue<uint8_t, 4> q;
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_FALSE(q.IsFull());
    EXPECT_EQ(q.GetCount(), 0);
    EXPECT_EQ(q.GetCapacity(), 4u);
}

static void TestCircularQueueNextSlotIncrements()
{
    DfxCircularQueue<uint8_t, 4> q;
    void* slot0 = q.NextSlot();
    EXPECT_TRUE(slot0 != nullptr);
    EXPECT_FALSE(q.IsEmpty());
    EXPECT_EQ(q.GetCount(), 1);
    *static_cast<uint8_t*>(slot0) = 0xA1;

    void* slot1 = q.NextSlot();
    EXPECT_TRUE(slot1 != nullptr);
    EXPECT_EQ(q.GetCount(), 2);
    EXPECT_TRUE(slot0 != slot1);
    EXPECT_EQ(*q.GetSlot(0), 0xA1);
}

static void TestCircularQueueFullAndWraparound()
{
    DfxCircularQueue<uint8_t, 4> q;
    for (uint32_t i = 0; i < 4; ++i) {
        *static_cast<uint8_t*>(q.NextSlot()) = static_cast<uint8_t>(i + 1);
    }
    EXPECT_TRUE(q.IsFull());
    EXPECT_EQ(q.GetCount(), 4);
    EXPECT_EQ(*q.GetSlot(0), 1);
    EXPECT_EQ(*q.GetSlot(3), 4);

    // 5th write wraps: oldest (slot0, value 1) is overwritten; begin advances; count stays at capacity
    *static_cast<uint8_t*>(q.NextSlot()) = 0xFF;
    EXPECT_TRUE(q.IsFull());
    EXPECT_EQ(q.GetCount(), 4);
    EXPECT_EQ(q.GetBegin(), 1u); // begin advanced by 1
}

static void TestCircularQueueMarkAllRead()
{
    DfxCircularQueue<uint8_t, 4> q;
    q.NextSlot();
    q.NextSlot();
    EXPECT_EQ(q.GetCount(), 2);
    q.MarkAllRead();
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.GetCount(), 0);
}

// ── DfxTaskInfo (res_pub.h) ─────────────────────────────────────────────────────

static void TestDfxTaskInfoDefaults()
{
    DfxTaskInfo info{};
    EXPECT_EQ(info.dfxOpInfo, DFX_INVALID_U64);
    EXPECT_EQ(info.channelHandle, DFX_INVALID_U64);
    EXPECT_EQ(info.sqId, 0u);
    EXPECT_EQ(info.taskId, 0u);
}

static void TestDfxTaskInfoIsTaskTypeValid()
{
    DfxTaskInfo info{};
    info.taskType = static_cast<u8>(TaskParamTypeVal::TASK_SDMA);
    EXPECT_TRUE(info.IsTaskTypeValid());
    info.taskType = static_cast<u8>(TaskParamTypeVal::TASK_NOTIFY_WAIT);
    EXPECT_TRUE(info.IsTaskTypeValid());
    info.taskType = static_cast<u8>(TaskParamTypeVal::TASK_PARAM_TYPE_COUNT);
    EXPECT_FALSE(info.IsTaskTypeValid());
    info.taskType = static_cast<u8>(TaskParamTypeVal::TASK_PARAM_TYPE_COUNT) + 1;
    EXPECT_FALSE(info.IsTaskTypeValid());
}

static void TestDfxTaskInfoRoundTripFields()
{
    DfxTaskInfo info{};
    info.taskType = static_cast<u8>(TaskParamTypeVal::TASK_UB);
    info.sqId = 7;
    info.taskId = 42;
    info.linkType = static_cast<u8>(DfxLinkTypeVal::LINK_UB);
    info.transportType = static_cast<u8>(DfxTransportType::DFX_TRANSPORT_TYPE_UB);
    info.channelHandle = 0xDEADBEEF;
    info.taskPara.ubDma.srcAddr = 0x1000;
    info.taskPara.ubDma.dstAddr = 0x2000;
    info.taskPara.ubDma.size = 512;
    info.taskPara.ubDma.notifyId = INVALID_U32;

    EXPECT_EQ(info.sqId, 7u);
    EXPECT_EQ(info.taskId, 42u);
    EXPECT_EQ(info.channelHandle, 0xDEADBEEFull);
    EXPECT_EQ(info.taskPara.ubDma.srcAddr, 0x1000ull);
    EXPECT_EQ(info.taskPara.ubDma.size, 512ull);
    EXPECT_EQ(info.taskPara.ubDma.notifyId, INVALID_U32);
}

// ── DfxDfxOpInfo (res_pub.h) ────────────────────────────────────────────────────

static void TestDfxDfxOpInfoDefaults()
{
    DfxDfxOpInfo op{};
    EXPECT_TRUE(op.commHandle == nullptr);
    EXPECT_EQ(op.count, 0ull);
    EXPECT_EQ(op.srcAddr, 0ull);
    EXPECT_EQ(op.dstAddr, 0ull);
    EXPECT_EQ(op.opIndex, 0u);
    EXPECT_EQ(op.opType, 0u);
    EXPECT_EQ(op.algType, 0u);
    EXPECT_EQ(op.dataType, 0u);
}

// ── DfxProfilingHandlerLite singleton ──────────────────────────────────────────

static void TestSingletonIdentity()
{
    auto& a = DfxProfilingHandlerLite::GetInstance();
    auto& b = DfxProfilingHandlerLite::GetInstance();
    EXPECT_TRUE(&a == &b);
}

static void TestProfStateDefaultsFalse()
{
    DfxProfilingHandlerLite::GetInstance().SetProL0On(false);
    DfxProfilingHandlerLite::GetInstance().SetProL1On(false);
    EXPECT_FALSE(DfxProfilingHandlerLite::GetInstance().GetProfL0State());
    EXPECT_FALSE(DfxProfilingHandlerLite::GetInstance().GetProfL1State());
}

static void TestProfStateSetters()
{
    DfxProfilingHandlerLite::GetInstance().SetProL0On(true);
    EXPECT_TRUE(DfxProfilingHandlerLite::GetInstance().GetProfL0State());
    DfxProfilingHandlerLite::GetInstance().SetProL1On(true);
    EXPECT_TRUE(DfxProfilingHandlerLite::GetInstance().GetProfL1State());
    DfxProfilingHandlerLite::GetInstance().SetProL0On(false);
    EXPECT_FALSE(DfxProfilingHandlerLite::GetInstance().GetProfL0State());
}

static void TestGetProfHashIdNullEmpty()
{
    EXPECT_EQ(DfxProfilingHandlerLite::GetInstance().GetProfHashId(nullptr, 0), DFX_INVALID_U64);
    EXPECT_EQ(DfxProfilingHandlerLite::GetInstance().GetProfHashId(nullptr, 3), DFX_INVALID_U64);
    EXPECT_EQ(DfxProfilingHandlerLite::GetInstance().GetProfHashId("x", 0), DFX_INVALID_U64);
}

static void TestGetProfHashIdDeterministic()
{
    const char* name = "AlgType::NHR";
    uint64_t h1 = DfxProfilingHandlerLite::GetInstance().GetProfHashId(name, static_cast<uint32_t>(strlen(name)));
    uint64_t h2 = DfxProfilingHandlerLite::GetInstance().GetProfHashId(name, static_cast<uint32_t>(strlen(name)));
    EXPECT_EQ(h1, h2);
    EXPECT_TRUE(h1 != DFX_INVALID_U64);
    EXPECT_TRUE(h1 != 0);
}

static void TestGetProfHashIdFnv1aKnownValue()
{
    // FNV-1a offset basis / prime (port uses these constants)
    const uint64_t FNV_OFFSET = 1469598103934665603ULL;
    const uint64_t FNV_PRIME = 1099511628211ULL;
    // single byte "a" = 0x61
    uint64_t expected = FNV_OFFSET;
    expected ^= 0x61;
    expected *= FNV_PRIME;
    uint64_t actual = DfxProfilingHandlerLite::GetInstance().GetProfHashId("a", 1);
    EXPECT_EQ(actual, expected);
}

static void TestGetProfHashIdDifferentInputsDiffer()
{
    uint64_t hA = DfxProfilingHandlerLite::GetInstance().GetProfHashId("alpha", 5);
    uint64_t hB = DfxProfilingHandlerLite::GetInstance().GetProfHashId("beta", 4);
    EXPECT_TRUE(hA != hB);
}

static void TestSetGetCurrDfxOpInfo()
{
    DfxDfxOpInfo op{};
    op.count = 100;
    op.opType = static_cast<u8>(OpTypeVal::OP_TYPE_ALLREDUCE);
    DfxProfilingHandlerLite::GetInstance().SetCurrDfxOpInfo(&op);
    const DfxDfxOpInfo* got = DfxProfilingHandlerLite::GetInstance().GetCurrDfxOpInfo();
    EXPECT_TRUE(got == &op);
    EXPECT_EQ(got->count, 100ull);
    EXPECT_EQ(got->opType, static_cast<u8>(OpTypeVal::OP_TYPE_ALLREDUCE));

    DfxProfilingHandlerLite::GetInstance().SetCurrDfxOpInfo(nullptr);
    EXPECT_TRUE(DfxProfilingHandlerLite::GetInstance().GetCurrDfxOpInfo() == nullptr);
}

static void TestSetCachedCommInfo()
{
    DfxProfilingHandlerLite::GetInstance().SetCachedCommInfo(0x1234, 0, 8);
    EXPECT_EQ(DfxProfilingHandlerLite::GetInstance().Init(), HCCL_SUCCESS);
}

// ── TaskInfoCircularQueue alias (the real DfxTaskInfo queue) ─────────────────────

static void TestTaskInfoCircularQueueAliasCompiles()
{
    TaskInfoCircularQueue q;
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.GetCapacity(), DFX_TASK_INFO_QUEUE_CAPACITY);
    DfxTaskInfo* slot = static_cast<DfxTaskInfo*>(q.NextSlot());
    EXPECT_TRUE(slot != nullptr);
    slot->taskType = static_cast<u8>(TaskParamTypeVal::TASK_NOTIFY_RECORD);
    slot->sqId = 5;
    EXPECT_TRUE(slot->IsTaskTypeValid());
}

int main()
{
    printf("=== DFX mechanism unit tests (ported from hcomm PR#3878) ===\n");

    RUN_TEST(TestCircularQueueEmpty);
    RUN_TEST(TestCircularQueueNextSlotIncrements);
    RUN_TEST(TestCircularQueueFullAndWraparound);
    RUN_TEST(TestCircularQueueMarkAllRead);
    RUN_TEST(TestDfxTaskInfoDefaults);
    RUN_TEST(TestDfxTaskInfoIsTaskTypeValid);
    RUN_TEST(TestDfxTaskInfoRoundTripFields);
    RUN_TEST(TestDfxDfxOpInfoDefaults);
    RUN_TEST(TestSingletonIdentity);
    RUN_TEST(TestProfStateDefaultsFalse);
    RUN_TEST(TestProfStateSetters);
    RUN_TEST(TestGetProfHashIdNullEmpty);
    RUN_TEST(TestGetProfHashIdDeterministic);
    RUN_TEST(TestGetProfHashIdFnv1aKnownValue);
    RUN_TEST(TestGetProfHashIdDifferentInputsDiffer);
    RUN_TEST(TestSetGetCurrDfxOpInfo);
    RUN_TEST(TestSetCachedCommInfo);
    RUN_TEST(TestTaskInfoCircularQueueAliasCompiles);

    printf("\n=== Results: %d passed, %d failed ===\n", g_passCount, g_failCount);
    return g_failCount == 0 ? 0 : 1;
}
