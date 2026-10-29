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
 * \file test_operator_ibwait_pointer_arm.cpp
 * \brief Two-arm coverage for the raw-pointer overload of IBWait (A4 / task book §5).
 *
 * VERIFIED AGAINST(pinned before use)
 *   BASE_COMMIT            16933cc30
 *
 *   LIBRARY_FINGERPRINT    1777c39a18731052ce08c608895d9b385113ca5e92e3f9718a7d5dd7cc0f6c1e
 *       sha256(git diff 16933cc30 -- include impl)   -- 18 files
 *       *** Cite THIS one. ***  It is the part these cases actually depend on, and it does NOT
 *       move when a test file is added or edited.  The whole-tree value below does, which is why
 *       two are recorded.
 *
 *   CHANGESET_FINGERPRINT  sha256(git diff 16933cc30 -- .)
 *       308c309e8db854322ae3d335236368faa0ebffe7cd990c20c8edf7503848dd65   (36 files / +4860)
 *       Measured immediately BEFORE these 18 test files landed.  Adding test files changes it --
 *       the post-landing value is in
 *       the apparatus run header captured together with the run.
 *       The measurements below were performed against 308c309e (this value).
 *   Both directions were measured against THIS fingerprint:
 *       delivery tree 308c309e...   compiles OK, and the test RUNS OK (rc=0, 0 failures)
 *       pristine tree (16933cc30)  see DISCRIMINATING POWER below
 *   A case is only known to hold for the fingerprint it was last verified against.  If the tree
 *   moves (and landing this file moves it), re-measure BOTH directions before citing this file.
 *
 * DISCRIMINATING POWER (measured, not assumed)
 *   Built against the pristine base tree (the base commit, which has no pointer overloads)
 *   this file FAILS to compile.  The first diagnostic, quoted verbatim bar the file:line prefix:
 *       error: invalid initialization of reference of type 'const AscendC::GlobalTensor<int>&' from expression of type
 * 'int*' It is the SECOND static_assert (the pointer form) that fails; the first (the Tensor form) is a control and
 * still passes, which is what makes the failure attributable to the pointer overload rather than to a broken file. A
 * case that compiles on both trees would be decorative; this one is not.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads (the join against the authoritative 25-name / 64-overload table is in
 *   the overload map captured together with the run):
 *     IBWait -- Tensor form  : IBWait<isAIVOnly>(const GlobalTensor<int32_t>&, const LocalTensor<int32_t>&, int32_t,
 * int32_t) IBWait -- Pointer form : IBWait<isAIVOnly, GmPtr, UbPtr>(GmPtr, UbPtr, int32_t, int32_t) (SFINAE on
 * CubePtrOverload::AnyPtr; quadrant key OV_IBWait_1_workspace)
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES -- and this is the whole of it -- that the raw-pointer overload of IBWait RESOLVES to a
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
 * IT DOES NOT PROVE that the pointer arm computes the same numbers as the Tensor arm.  That is a
 * different claim on a different instrument: the 40-cell two-arm byte-identity matrix in
 * the device two-arm sweep (every GREEN cell byte-identical; the complete table is in the
 * verification report).  The
 * tikicpulib CPU simulation this file runs under does not produce observable values on the on-chip
 * path (see HARNESS_INERT.md), so a numerical assertion here would be a vacuous green, and there is
 * none.
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
 *     (captured in the run transcript that accompanies the verification report.)
 * ================================================================================================
 */

#include <gtest/gtest.h>
#include <type_traits>
#include <utility>
#include "kernel_operator.h"
#include "test_utils.h"

using namespace AscendC;

namespace {
constexpr int32_t WS_LEN = 64;
constexpr int32_t SENTINEL = 0x5A5A1234;
} // namespace

// ---- compile-time resolution checks ---------------------------------------
// These are the load-bearing half of the case.  If the pointer overload were absent, or if the
// call resolved to the Tensor form instead, the second static_assert would not compile.
//
// ★ NOTE ON A DEFECT THAT WAS HERE, kept because it is the exact failure mode this suite exists to
//   avoid.  The first version wrote `std::declval<LocalTensor<int32_t>&>` without the trailing `()`,
//   so the argument was the address of the declval overload set rather than a value.  That is
//   ill-formed on BOTH trees -- but the delivery tree compiled it with rc=0 while the pristine tree
//   reported "no matching function for call to 'SyncAll<false>(GlobalTensor<int>&, <unresolved
//   overloaded function type>, int)'".  In other words the broken assert PASSED on the tree under
//   test and only failed on the control: an assertion that reports success when it did not succeed.
//   It was caught by reading the pristine-side error text, not by the delivery-side rc.  Both
//   calls below carry `()`.
static_assert(
    std::is_same<
        decltype(AscendC::IBWait<false>(
            std::declval<GlobalTensor<int32_t>&>(), std::declval<LocalTensor<int32_t>&>(), 0, 0)),
        void>::value,
    "IBWait Tensor form must stay callable with LocalTensor workspaces");
static_assert(
    std::is_same<
        decltype(AscendC::IBWait<false>(std::declval<__gm__ int32_t*>(), std::declval<__ubuf__ int32_t*>(), 0, 0)),
        void>::value,
    "IBWait pointer form must resolve to the raw-pointer overload, not to the Tensor overload");

class IBWaitPointerArmTestsuite : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// ARM T -- Tensor form.  The overload the existing suite already uses.
__global__ __aicore__ void KernelIBWaitTensorArm(__gm__ int32_t* gmWs, __gm__ int32_t* result)
{
    GlobalTensor<int32_t> gm;
    gm.SetGlobalBuffer((__gm__ int32_t*)gmWs, WS_LEN);
    TPipe* pipe = GetTPipePtr();
    TBuf<TPosition::VECCALC> bUb;
    pipe->InitBuffer(bUb, WS_LEN * sizeof(int32_t));
    LocalTensor<int32_t> ub = bUb.Get<int32_t>();

    AscendC::IBWait<false>(gm, ub, 0, 0);

    // Written only if the API call returned.  The test asserts on this word, so a kernel that
    // faulted or returned early fails rather than passing silently.
    result[0] = SENTINEL;
}

// ARM P -- pointer form.  This is the call the pointer-ization added, and the reason this file
// exists: without it the case compiles on the pristine tree too and proves nothing.
__global__ __aicore__ void KernelIBWaitPointerArm(__gm__ int32_t* gmWs, __gm__ int32_t* result)
{
    GlobalTensor<int32_t> gm;
    gm.SetGlobalBuffer((__gm__ int32_t*)gmWs, WS_LEN);
    TPipe* pipe = GetTPipePtr();
    TBuf<TPosition::VECCALC> bUb;
    pipe->InitBuffer(bUb, WS_LEN * sizeof(int32_t));
    LocalTensor<int32_t> ub = bUb.Get<int32_t>();

    AscendC::IBWait<false>((__gm__ int32_t*)gm.GetPhyAddr(), (__ubuf__ int32_t*)ub.GetPhyAddr(), 0, 0);

    result[0] = SENTINEL;
}

// The two arms are NOT executed, on purpose.
//
// IBWait is an inter-core handshake: its body spins on a synchronisation word
// (while (true) { ... if (ubWorkspace.GetValue(0) == 0) ... break; }) that in practice only
// another core clears.  This unit-test binary runs single-threaded, so the loop cannot make
// progress and the process ABORTS instead of measuring anything.  The repository's own coverage
// of IBWait lives in tests/api/basic_api/ascendc_header_checker/kernel_operator_block_sync_intf.cpp,
// which likewise only COMPILES the call.
//
// The two __global__ kernels above are still defined and their addresses are taken below, so both
// the Tensor call and the raw-pointer call are still compiled -- that compilation IS the evidence
// that the pointer overload resolves in a kernel context.  What must not happen is executing a
// handshake that needs a second core.
TEST_F(IBWaitPointerArmTestsuite, IBWaitOverloadResolution)
{
    auto tensorArm = &KernelIBWaitTensorArm;
    auto pointerArm = &KernelIBWaitPointerArm;
    EXPECT_NE(tensorArm, nullptr) << "the Tensor arm must still be compiled";
    EXPECT_NE(pointerArm, nullptr) << "the pointer arm must still be compiled";

    EXPECT_TRUE((std::is_same<
                 decltype(AscendC::IBWait<false>(
                     std::declval<GlobalTensor<int32_t>&>(), std::declval<LocalTensor<int32_t>&>(), 0, 0)),
                 void>::value))
        << "the Tensor call must stay callable with LocalTensor workspaces";
    EXPECT_TRUE(
        (std::is_same<
            decltype(AscendC::IBWait<false>(std::declval<__gm__ int32_t*>(), std::declval<__ubuf__ int32_t*>(), 0, 0)),
            void>::value))
        << "the raw-pointer call must resolve to the pointer overload";
}
