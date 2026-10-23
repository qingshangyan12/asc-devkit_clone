/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file test_operator_dumptensor_local_pointer_arm.cpp
 * \brief Pointer-arm call coverage for the DumpTensor LocalTensor overloads (task book 2.4 rows 5 and 6).
 *
 * WHAT THIS FILE IS
 *   A compile-time CALL-COVERAGE case, the same nature as the other A4 pointer-arm cases: it
 *   proves the raw-pointer overloads exist, are reachable, and reach the gate the Tensor form
 *   reaches.  It makes no numerical claim.
 *
 * COVERAGE CLAIM
 *     DumpTensor[3] Local -- Tensor form  : DumpTensor<T>(const LocalTensor<T>&, uint32_t, uint32_t)
 *                           Pointer form : DumpTensor<TPosition, T>(DstPtr, uint32_t, uint32_t)
 *     DumpTensor[4] Local -- Tensor form  : ... + const ShapeInfo&
 *                           Pointer form : DumpTensor<TPosition, T>(DstPtr, uint32_t, uint32_t, const ShapeInfo&)
 *   The pointer arm is exercised for three roles -- UB (VECCALC), L1 (A1) and L0C (CO1) -- which are
 *   the three the dav_3510 dump impl actually branches on for real transfers.
 *
 * WHY THE POSITION IS A TEMPLATE ARGUMENT
 *   Under ASCENDC_CPU_DEBUG the platform defines __ubuf__ / __cbuf__ / __cc__ / __gm__ as empty
 *   macros, so an overload per address space would collapse into one signature; naming the position
 *   keeps one form both builds can call, which is what makes this coverage real rather than nominal.
 *
 * THE GATE IS REACHED, NOT BYPASSED
 *   The pointer overloads forward into the Tensor form, which on dav-3510 takes the arch-keyed
 *   "not supported in cpu mode" assert.  So each arm raises exactly once; the count is asserted
 *   below, so "it ran" cannot be confused with "an assert fired and nobody looked".
 */
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include <type_traits>
#include <utility>
#include "kernel_operator.h"
#include "test_utils.h"

using namespace AscendC;

namespace {
constexpr int32_t DUMP_LEN = 64;
constexpr int32_t DUMP_WS = 1 << 20;
constexpr int32_t SENTINEL = 0x5A5A1234;
int32_t g_raiseCount = 0;
} // namespace

static int32_t RaiseStub(int32_t)
{
    g_raiseCount++;
    return 0;
}

class DumpTensorLocalPointerArmTestsuite : public testing::Test {
protected:
    void SetUp()
    {
        MOCKER(raise, int32_t(*)(int32_t)).stubs().will(invoke(RaiseStub));
        savedCore_ = g_coreType;
        savedBN_ = block_num;
        savedBI_ = block_idx;
        savedSBI_ = sub_block_idx;
        savedTR_ = g_taskRation;
        savedDump_ = AscendC::g_dumpWorkspaceReserved;
        savedSys_ = GetSysWorkSpacePtr();
        g_coreType = AIC_TYPE;
        SetGCoreType(1);
        block_num = 1;
        block_idx = 0;
        sub_block_idx = 0;
        g_taskRation = 1;
        static uint8_t ws[DUMP_WS] = {0};
        AscendC::g_dumpWorkspaceReserved = (__gm__ uint8_t*)ws;
        SetSysWorkSpacePtr((__gm__ uint8_t*)ws);
    }
    void TearDown()
    {
        SetGCoreType(savedCore_);
        block_num = savedBN_;
        block_idx = savedBI_;
        sub_block_idx = savedSBI_;
        g_taskRation = savedTR_;
        AscendC::g_dumpWorkspaceReserved = savedDump_;
        SetSysWorkSpacePtr(savedSys_);
        GlobalMockObject::verify();
    }

private:
    int32_t savedCore_;
    decltype(block_num) savedBN_;
    decltype(block_idx) savedBI_;
    decltype(sub_block_idx) savedSBI_;
    decltype(g_taskRation) savedTR_;
    __gm__ uint8_t* savedDump_;
    __gm__ uint8_t* savedSys_;
};

// ---- compile-time resolution checks ---------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::DumpTensor<TPosition::VECCALC, float>(
            std::declval<float*>(), std::declval<uint32_t>(), std::declval<uint32_t>())),
        void>::value,
    "DumpTensor[3] Local pointer form must resolve");
static_assert(
    std::is_same<
        decltype(AscendC::DumpTensor<TPosition::VECCALC, float>(
            std::declval<float*>(), std::declval<uint32_t>(), std::declval<uint32_t>(),
            std::declval<const ShapeInfo&>())),
        void>::value,
    "DumpTensor[4] Local pointer form must resolve");

// ARM P -- the pointer arm.  Each role is named by the TPosition template argument, and the
// address-space cast is the one a caller would use to obtain the address from a LocalTensor.
__global__ __aicore__ void K_DumpTensor3_P_Ub(__gm__ float* p, __gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(t, float, VECCALC, 0, DUMP_LEN)
    AscendC::DumpTensor<TPosition::VECCALC, float>((__ubuf__ float*)t.GetPhyAddr(), 0u, 16u);
    r[0] = SENTINEL;
}
__global__ __aicore__ void K_DumpTensor4_P_Ub(__gm__ float* p, __gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(t, float, VECCALC, 0, DUMP_LEN)
    ShapeInfo si{};
    AscendC::DumpTensor<TPosition::VECCALC, float>((__ubuf__ float*)t.GetPhyAddr(), 0u, 16u, si);
    r[0] = SENTINEL;
}
__global__ __aicore__ void K_DumpTensor3_P_L1(__gm__ float* p, __gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(t, float, A1, 0, DUMP_LEN)
    AscendC::DumpTensor<TPosition::A1, float>((__cbuf__ float*)t.GetPhyAddr(), 0u, 16u);
    r[0] = SENTINEL;
}
__global__ __aicore__ void K_DumpTensor3_P_L0C(__gm__ float* p, __gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(t, float, CO1, 0, DUMP_LEN)
    AscendC::DumpTensor<TPosition::CO1, float>((__cc__ float*)t.GetPhyAddr(), 0u, 16u);
    r[0] = SENTINEL;
}

// Each case asserts TWO things: that the kernel body reached its end (the sentinel), and that the
// architecture cpu-mode assertion fired exactly once -- which is what distinguishes "the pointer
// call reached the gate the Tensor form reaches" from "the call silently bound elsewhere".
#define DUMP_PTR_RUN(name)                                           \
    static float gm[DUMP_LEN] = {0};                                 \
    static int32_t r[1] = {0};                                       \
    g_raiseCount = 0;                                                \
    name(gm, r);                                                     \
    EXPECT_EQ(r[0], SENTINEL) << #name " did not run to completion"; \
    EXPECT_EQ(g_raiseCount, 1) << #name ": expected exactly one cpu-mode assertion"

TEST_F(DumpTensorLocalPointerArmTestsuite, DumpTensor3PointerUbRuns) { DUMP_PTR_RUN(K_DumpTensor3_P_Ub); }
TEST_F(DumpTensorLocalPointerArmTestsuite, DumpTensor4PointerUbRuns) { DUMP_PTR_RUN(K_DumpTensor4_P_Ub); }
TEST_F(DumpTensorLocalPointerArmTestsuite, DumpTensor3PointerL1Runs) { DUMP_PTR_RUN(K_DumpTensor3_P_L1); }
TEST_F(DumpTensorLocalPointerArmTestsuite, DumpTensor3PointerL0CRuns) { DUMP_PTR_RUN(K_DumpTensor3_P_L0C); }
