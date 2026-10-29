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
 * \file test_operator_initoutput.cpp
 * \brief Two-arm coverage for InitOutput (A4, spec overload 1 of 1).
 *
 * DISCRIMINATING POWER (measured, not assumed)
 *   Built against the pristine base tree (the base commit, which has no pointer overloads)
 *   this file FAILS to compile.  The first diagnostic, quoted verbatim bar the file:line prefix:
 *       error: could not convert 'std::declval<float*>()' from 'float*' to 'AscendC::GlobalTensor<float>'
 *   It is the SECOND static_assert (the pointer form) that fails; the first (the Tensor form) is a
 *   control and still passes, which is what makes the failure attributable to the pointer overload
 *   rather than to a broken file.
 *   A case that compiles on both trees would be decorative; this one is not.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads:
 *     InitOutput[1] -- Tensor form  : InitOutput<T>(GlobalTensor<T>, uint32_t, T)
 *     InitOutput[1] -- Pointer form : InitOutput<T>(__gm__ T*, uint32_t, T)
 *                  (SFINAE on CubePtrOverload::AnyPtr; quadrant key OV_InitOutput_1_Global)
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES -- and this is the whole of it -- that the raw-pointer overload of InitOutput[1] RESOLVES to a
 * declaration on the tree under test, and that a kernel which ACTUALLY CALLS that overload reaches
 * the end of its body.  "Covered" therefore means: the overload is called by a launched kernel and
 * the test passes.  It does NOT mean "this file compiles" -- a file that only compiles is
 * NOT_COVERED and must not be counted.
 *
 * BOTH ARMS PASSING IS NOT A STATEMENT ABOUT THE NUMBERS.  This file measures CALL COVERAGE --
 * that the overload resolves and that a kernel which calls it runs to completion.  Numerical
 * equivalence is a different gate on a different instrument (A2, on device).  A reader who takes
 * "2 tests passed" out of this file as "the numerics are right" has read the wrong file.
 *
 * IT DOES NOT PROVE that the pointer arm computes the same numbers as the Tensor arm.  The
 * tikicpulib CPU simulation this file runs under does not execute the on-chip datapath, so a
 * numerical assertion here would pass whatever the API did; there is none.
 *
 * Two rules this file obeys, written down because they are the reason it is allowed to exist:
 *   * A case that compiles on BOTH the pristine tree and the delivery tree is decorative.  The
 *     discriminating power of this file is MEASURED, not assumed -- see DISCRIMINATING POWER.
 *   * A case that CANNOT FAIL does not belong in the suite.  Each arm below asserts an observable
 *     side effect of the kernel body (a sentinel word written after the API call returns), so a
 *     kernel that silently did nothing fails the test.  That sentinel is written by THIS test's own
 *     code, not by the API -- which is why it proves "the kernel body ran to the end" and nothing
 *     more about the API's behaviour.  The two claims are kept separate on purpose.
 *     This was MEASURED, not assumed: with the sentinel write deleted from the pointer-arm kernel,
 *     the test fails --
 *         the Pointer-arm kernel body did not run to completion
 *         [  FAILED  ] ...PointerArmRuns
 * ================================================================================================
 */

#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "kernel_operator.h"
#include "test_utils.h"

using namespace AscendC;

namespace {
constexpr uint32_t IO_LEN = 256;
constexpr int32_t SENTINEL = 0x5A5A1234;
} // namespace

// ---- compile-time resolution checks ---------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::InitOutput<float>(
            std::declval<GlobalTensor<float>&>(), std::declval<uint32_t>(), std::declval<float>())),
        void>::value,
    "InitOutput Tensor form must resolve");
static_assert(
    std::is_same<
        decltype(AscendC::InitOutput<float>(
            std::declval<__gm__ float*>(), std::declval<uint32_t>(), std::declval<float>())),
        void>::value,
    "InitOutput pointer form must resolve to the raw-pointer overload");

class InitOutputTestsuite : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// ARM T -- Tensor form.
__global__ __aicore__ void KernelInitOutputTensorArm(__gm__ float* ws, __gm__ int32_t* result)
{
    GlobalTensor<float> gm;
    gm.SetGlobalBuffer((__gm__ float*)ws, IO_LEN);

    AscendC::InitOutput<float>(gm, IO_LEN, 1.5f);

    result[0] = SENTINEL;
}

// ARM P -- pointer form.
__global__ __aicore__ void KernelInitOutputPointerArm(__gm__ float* ws, __gm__ int32_t* result)
{
    GlobalTensor<float> gm;
    gm.SetGlobalBuffer((__gm__ float*)ws, IO_LEN);

    AscendC::InitOutput<float>((__gm__ float*)gm.GetPhyAddr(), IO_LEN, 1.5f);

    result[0] = SENTINEL;
}

TEST_F(InitOutputTestsuite, InitOutputTensorArmRuns)
{
    static float ws[IO_LEN] = {0};
    static int32_t result[1] = {0};
    KernelInitOutputTensorArm(ws, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Tensor-arm kernel body did not run to completion";
}

TEST_F(InitOutputTestsuite, InitOutputPointerArmRuns)
{
    static float ws[IO_LEN] = {0};
    static int32_t result[1] = {0};
    KernelInitOutputPointerArm(ws, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Pointer-arm kernel body did not run to completion";
}
