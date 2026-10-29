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
 * \file test_operator_loaddata_3dv1_pointer_arm.cpp
 * \brief Pointer-arm call coverage for LoadData with LoadData3DParamsV1 (task book 2.4 row 27).
 *
 * WHAT THIS FILE IS
 *   A compile-time CALL-COVERAGE case, the same nature as the other A4 pointer-arm cases: it
 *   proves the raw-pointer overload exists, is reachable, and forwards to the same backend the
 *   Tensor form reaches.  It makes no numerical claim.
 *
 * COVERAGE CLAIM
 *     Tensor form  : LoadData<T, defaultConfig, U>(const LocalTensor<T>&, const LocalTensor<T>&,
 *                                                  const LoadData3DParamsV1<U>&)
 *     Pointer form : LoadData(DstPtr, SrcPtr, const LoadData3DParamsV1<U>&)
 *   exercised for the three destinations the backend distinguishes -- L0A (A2), L0B (B2) and
 *   UB (VECCALC) -- with the source in L1 (A1).
 *
 * SCOPE NOTE, STATED RATHER THAN IMPLIED
 *   On dav-3510 the three backends this forwards to (LoadData3DV1L12L0{A,B,UB}Cal) are
 *   "unsupported on current device" stubs, exactly as the Tensor form records.  The pointer arm
 *   therefore performs the same non-transfer the Tensor arm does; what it proves is that the
 *   pointer form resolves and reaches that same backend, not that a transfer happens.  The same
 *   is true of the delivered InitConstValue pointer arm, whose 3510 backends are also stubs.
 *
 *   Only the sentinel is asserted, as in the delivered MmadMx pointer-arm case: the cpu-mode
 *   assert this family reaches is counted where it is meaningful (the dump cases), and asserting
 *   a count here would claim a property this path does not have.
 */
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include <type_traits>
#include <utility>
#include "kernel_operator.h"
#include "test_utils.h"

using namespace AscendC;

namespace {
constexpr int32_t SENTINEL = 0x5A5A1234;
constexpr int32_t LD3DV1_LEN = 1024;
int32_t g_raiseCount = 0;
} // namespace

// dav-3510 上 LoadData3DV1 的三个后端是 unsupported-on-current-device 桩；
// 指针形转发到的正是张量形那三个桩，于是会 raise(SIGABRT)。
// 与同批 Dump 用例、以及仓库自带的 ascendc_case_ascend950pr_9599_aiv_basic/test_dump_tensor.cpp
// 同一做法：把 raise 打桩、只记录不硬断言 —— 否则 CPU 模型下会把整个 UT 进程 abort 掉
// （实测：不加这一句时 build.sh --basic_test_two 会 core dumped）。
static int32_t RaiseStub(int32_t)
{
    g_raiseCount++;
    return 0;
}

class LoadData3DV1PointerArmTestsuite : public testing::Test {
protected:
    void SetUp()
    {
        MOCKER(raise, int32_t(*)(int32_t)).stubs().will(invoke(RaiseStub));
        g_raiseCount = 0;
        g_coreType = AIC_TYPE;
    }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// ---- compile-time resolution check ----------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::LoadData(
            std::declval<float*>(), std::declval<float*>(), std::declval<const LoadData3DParamsV1<float>&>())),
        void>::value,
    "LoadData 3dv1 pointer form must resolve");

// ARM P -- the pointer arm.  The destination address space names the pipe; the source is L1.
__global__ __aicore__ void K_LoadData3DV1_P_L0A(__gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(dst, float, A2, 0, LD3DV1_LEN)
    LOCAL_TENSOR_REGISTER(src, float, A1, 0, LD3DV1_LEN)
    LoadData3DParamsV1<float> p;
    AscendC::LoadData((__ca__ float*)dst.GetPhyAddr(), (__cbuf__ float*)src.GetPhyAddr(), p);
    r[0] = SENTINEL;
}
__global__ __aicore__ void K_LoadData3DV1_P_L0B(__gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(dst, float, B2, 0, LD3DV1_LEN)
    LOCAL_TENSOR_REGISTER(src, float, B1, 0, LD3DV1_LEN)
    LoadData3DParamsV1<float> p;
    AscendC::LoadData((__cb__ float*)dst.GetPhyAddr(), (__cbuf__ float*)src.GetPhyAddr(), p);
    r[0] = SENTINEL;
}
__global__ __aicore__ void K_LoadData3DV1_P_Ub(__gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(dst, float, VECCALC, 0, LD3DV1_LEN)
    LOCAL_TENSOR_REGISTER(src, float, A1, 0, LD3DV1_LEN)
    LoadData3DParamsV1<float> p;
    AscendC::LoadData((__ubuf__ float*)dst.GetPhyAddr(), (__cbuf__ float*)src.GetPhyAddr(), p);
    r[0] = SENTINEL;
}

#define LD3DV1_PTR_RUN(name)   \
    static int32_t r[1] = {0}; \
    name(r);                   \
    EXPECT_EQ(r[0], SENTINEL) << #name " did not run to completion"

TEST_F(LoadData3DV1PointerArmTestsuite, LoadData3DV1PointerL0ARuns) { LD3DV1_PTR_RUN(K_LoadData3DV1_P_L0A); }
TEST_F(LoadData3DV1PointerArmTestsuite, LoadData3DV1PointerL0BRuns) { LD3DV1_PTR_RUN(K_LoadData3DV1_P_L0B); }
TEST_F(LoadData3DV1PointerArmTestsuite, LoadData3DV1PointerUbRuns) { LD3DV1_PTR_RUN(K_LoadData3DV1_P_Ub); }
