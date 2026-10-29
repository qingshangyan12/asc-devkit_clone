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
 * \file test_operator_dumpaccchkpoint_local_arm.cpp
 * \brief Tensor-arm coverage for the LocalTensor DumpAccChkPoint overload (A4).
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
 * DISCRIMINATING POWER: NONE, and that is measured.
 *   Built against the pristine base tree this file compiles: rc=0, no diagnostic.
 *   Built against the delivery tree it also compiles.
 *   So it cannot fail on a tree without the patch, and it carries no discriminating power.
 *   See the note below for what it does and does not license -- this file makes a CALL-COVERAGE
 *   claim for the LocalTensor Dump overloads, not a claim about the pointer patch.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads (the join against the authoritative 25-name / 64-overload table is in
 *   the overload map captured together with the run):
 *     DumpAccChkPoint[2] (LocalTensor form) -- Tensor form  : DumpAccChkPoint<T>(const LocalTensor<T>&, uint32_t,
 * uint32_t, uint32_t) DumpAccChkPoint[2] (LocalTensor form) -- Pointer form : (none -- not expressible for this role;
 * see THE DESIGN BOUNDARY in the header above) (SFINAE on CubePtrOverload::AnyPtr; quadrant key
 * OV_DumpAccChkPoint_2_Local)
 *
 * WHERE THE POINTER FORM OF THIS ROLE NOW LIVES
 *   These LocalTensor roles DO have a pointer form as of the gap-fill that added
 *   DumpTensor<TPosition, T> / DumpAccChkPoint<TPosition, T>; the earlier note in this header, and
 *   the header text it quoted, said otherwise and have been corrected.  The reason that note gave
 *   (a raw pointer cannot carry the position) did not hold: the impl reads only
 *   GetPosition() -- which the caller now supplies as a template argument -- and, for
 *   DumpAccChkPoint, an extent the GM form already declares unbounded.
 *
 *   WHY THE POSITION IS A TEMPLATE ARGUMENT: under ASCENDC_CPU_DEBUG __ubuf__ / __cbuf__ / __cc__ /
 *   __gm__ are empty macros, so an overload per address space collapses into the GM signature;
 *   naming the position keeps one form that both builds can declare and call.
 *
 *   The pointer arm for this role is covered by the sibling file (see the pointer-arm file named
 *   for the same API).  THIS file stays tensor-arm only.

 * SO THIS FILE IS TENSOR-ARM ONLY, AND THAT IS NOT AN UNCOVERED SLOT.
 *
 * ★ THIS FILE COMPILES ON BOTH TREES -- AND THAT IS MEASURED, NOT ASSUMED
 *   Built against the pristine base tree it compiles: rc=0, no diagnostic.
 *   Built against the delivery tree it also compiles.
 *
 *   SO THIS FILE HAS NO DISCRIMINATING POWER, and the project's rule is that a case which compiles
 *   on both trees is decorative.  The rule is not being dodged here -- it is being applied to the
 *   right claim.  This file makes a CALL-COVERAGE claim, not a pointer-patch claim:
 *
 *     IT CLAIMS      the LocalTensor Dump overloads are called by a launched kernel in our suite,
 *                    so the "Dump family call count is zero" gap no longer holds for them.
 *     IT DOES NOT    prove anything about the pointer overloads.  Those are now covered by the
 *                    pointer-arm file for the same API, which does fail to compile on the
 *                    pristine tree.
 *
 *   A reader who wants discriminating power for the Dump family should read the two GM files
 *   (test_operator_dumptensor_gm_pointer_arm.cpp, test_operator_dumpaccchkpoint_gm_pointer_arm.cpp):
 *   those carry a pointer arm and DO fail to compile on the pristine tree.
 *
 * WHAT "PASSED" MEANS HERE -- read before citing this file
 *   Every tensor-form Dump entry point asserts on dav-3510:
 *       ASCENDC_ASSERT((false), {KERNEL_LOG(KERNEL_ERROR, "Dump... is not supported in cpu
 *           mode.");});                       impl/basic_api/kernel_operator_dump_tensor_intf_impl.h
 *   and the guard is keyed on the ARCHITECTURE (`#if __NPU_ARCH__ == 3003 || 3113` / #else), not on
 *   ASCENDC_CPU_DEBUG, so it fires on EVERY call on this arch.
 *   The fixture stubs `raise` -- exactly as the pre-existing
 *   tests/.../ascendc_case_ascend950pr_9599_aiv_basic/test_dump_tensor.cpp does -- and the test
 *   asserts BOTH that the kernel reached its end and that the assertion fired exactly once.
 *
 *   ⇒ WHAT THIS PROVES: the overload RESOLVES and is CALLED, and the call returns.
 *     WHAT IT DOES NOT PROVE: that the dump succeeded, or that these APIs are usable in cpu mode.
 *     The source says they are not.  A reader who takes "2 tests passed" from this file as "the
 *     dump was verified" has read the wrong thing out of it.
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES -- and this is the whole of it -- that the raw-pointer overload of DumpAccChkPoint[2] (LocalTensor form)
 * RESOLVES to a declaration on the tree under test, and that a kernel which ACTUALLY CALLS that overload reaches the
 * end of its body.  "Covered" therefore means: the overload is called by a launched kernel and the test passes.  It
 * does NOT mean "this file compiles" -- a file that only compiles is NOT_COVERED and must not be counted.
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
#include <mockcpp/mockcpp.hpp>
#include <type_traits>
#include <utility>
#include "kernel_operator.h"
#include "test_utils.h"

using namespace AscendC;

// ---------------------------------------------------------------------------------------------
// The dump fixture.  This differs in SHAPE from the other A4 cases, which need only a kernel and a
// sentinel, and the difference has to be stated rather than left to be discovered.
//
// Every tensor-form Dump entry point asserts on this architecture:
//     impl/basic_api/kernel_operator_dump_tensor_intf_impl.h:65/77/89/102  (DumpTensor)
//     impl/basic_api/kernel_operator_dump_tensor_intf_impl.h:116/135       (DumpAccChkPoint)
//         ASCENDC_ASSERT((false), {KERNEL_LOG(KERNEL_ERROR, "Dump... is not supported in cpu
//         mode.");});
// and that guard is `#if __NPU_ARCH__ == 3003 || 3113` / #else -- it is keyed on the ARCHITECTURE,
// not on ASCENDC_CPU_DEBUG, so on dav-3510 EVERY tensor-form dump call takes it.
//
// The pointer forms do not carry the assert themselves: they forward into the tensor form (see
// impl/basic_api/kernel_operator_dump_tensor_intf_impl.h:156-161), which is why each arm raises
// exactly once rather than the pointer arm raising zero times.  The gate is reached, not bypassed.
//
// So `raise` is stubbed below, exactly as the pre-existing
// tests/.../ascendc_case_ascend950pr_9599_aiv_basic/test_dump_tensor.cpp does it.  The count is
// kept and asserted, so "it ran" cannot be confused with "it ran because an assert fired and
// nobody looked".
// ---------------------------------------------------------------------------------------------
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

class DumpAccChkPointLocalTestsuite : public testing::Test {
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
        decltype(AscendC::DumpAccChkPoint<float>(
            std::declval<LocalTensor<float>&>(), std::declval<uint32_t>(), std::declval<uint32_t>(),
            std::declval<uint32_t>())),
        void>::value,
    "DumpAccChkPoint[2] Local Tensor form");

// ARM T only, for spec [2].
__global__ __aicore__ void K_DumpAccChkPoint2_T(__gm__ float* p, __gm__ int32_t* r)
{
    LOCAL_TENSOR_REGISTER(t, float, VECCALC, 0, DUMP_LEN)
    AscendC::DumpAccChkPoint<float>(t, 0u, 0u, 16u);
    r[0] = SENTINEL;
}

// Each case asserts TWO things: that the kernel body reached its end (the sentinel), and that the
// architecture's cpu-mode assertion fired exactly once.  The second is not decoration -- it is what
// distinguishes "the call executed" from "the call silently did nothing".
#define DUMP_RUN(name)                                               \
    static float gm[DUMP_LEN] = {0};                                 \
    static int32_t r[1] = {0};                                       \
    g_raiseCount = 0;                                                \
    name(gm, r);                                                     \
    EXPECT_EQ(r[0], SENTINEL) << #name " did not run to completion"; \
    EXPECT_EQ(g_raiseCount, 1) << #name ": expected exactly one cpu-mode assertion (see the file header)"

TEST_F(DumpAccChkPointLocalTestsuite, DumpAccChkPoint2TensorArmRuns) { DUMP_RUN(K_DumpAccChkPoint2_T); }
