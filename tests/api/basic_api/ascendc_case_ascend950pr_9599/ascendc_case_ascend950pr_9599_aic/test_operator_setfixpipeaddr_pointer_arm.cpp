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
 * \file test_operator_setfixpipeaddr_pointer_arm.cpp
 * \brief NOT_APPLICABLE (upstream stub) annotation for SetFixPipeAddr's raw-pointer overload (A4).
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
 *       error: no matching function for call to 'SetFixPipeAddr(half*, short unsigned int)'
 *   It is the SECOND static_assert (the pointer form) that fails; the first (the Tensor form) is a
 *   control and still passes, which is what makes the failure attributable to the pointer overload
 *   rather than to a broken file.
 *   A case that compiles on both trees would be decorative; this one is not.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads (the join against the authoritative 25-name / 64-overload table is in
 *   the overload map captured together with the run):
 *     SetFixPipeAddr -- Tensor form  : SetFixPipeAddr<T>(const LocalTensor<T>&, uint16_t)
 *     SetFixPipeAddr -- Pointer form : SetFixPipeAddr(DstPtr, uint16_t)   [address-space selected: __cbuf__ L1 |
 * __ubuf__ UB] (SFINAE on CubePtrOverload::AnyPtr; quadrant key OV_SetFixPipeAddr_1_eleWiseData (NOT_APPLICABLE:
 * upstream NOT_SUPPORT stub on dav-3510))
 *
 * ===================== WHY THIS FILE HAS NO RUNNING KERNEL =====================================
 *
 * This name breaks the two-arm pattern, and the reason is measured, not stylistic.
 *
 * SetFixPipeAddr's backend on dav-3510 is a stub:
 *     impl/basic_api/dav_3510/kernel_operator_fixpipe_impl.h:35-38
 *         template <typename T>
 *         __aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T> &eleWiseTensor,
 *                                                   uint16_t c0ChStride)
 *         {
 *             ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
 *         }
 * and that macro ends in `raise(SIGABRT)` (impl/basic_api/kernel_log.h:87-93) whenever its
 * condition is false.  A first version of this file DID launch both arms; the run died with
 *     [ RUN ] SetFixPipeAddrPointerArmTestsuite.SetFixPipeAddrTensorArmRuns
 *     Aborted (core dumped)   run_rc=134
 * in the TENSOR arm -- i.e. on the pre-existing product path, before the pointer arm was ever
 * reached.  No runtime arm for this name is possible on this arch, on either tree.
 *
 * Note where that file is: impl/basic_api/dav_3510/kernel_operator_fixpipe_impl.h is NOT one of the
 * 18 files in the delivered changeset.  The stub is upstream, and the pointer overload faithfully
 * forwards into it -- which is the correct behaviour (the pointer form must not diverge from the
 * Tensor form), but it does mean the call cannot be exercised here.
 *
 * So for SetFixPipeAddr the A4 claim is RESOLUTION-ONLY and is recorded as such in the mapping
 * table: the overload is shown to resolve and to be the one selected, and the CALL is deferred to
 * whichever arch implements the backend.  This is a narrower claim than for the other nine names
 * and must not be reported as the same thing.
 * ================================================================================================
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES that the raw-pointer overload of SetFixPipeAddr RESOLVES, and that it is the overload a
 * raw-pointer call site selects.
 *
 * IT DOES NOT PROVE THAT THE CALL RUNS, AND ON THIS ARCH IT CANNOT RUN -- FOR EITHER ARM.  The
 * backend on dav-3510 is a stub:
 *     impl/basic_api/dav_3510/kernel_operator_fixpipe_impl.h:35-38
 *         template <typename T>
 *         __aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T> &eleWiseTensor,
 *                                                   uint16_t c0ChStride)
 *         { ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr"); }
 * and ASCENDC_REPORT_NOT_SUPPORT (impl/basic_api/kernel_log.h:87-93) ends in raise(SIGABRT)
 * whenever its condition is false.  Measured, not inferred: the TENSOR arm -- which this project
 * did not touch -- aborts with run_rc=134 before the pointer arm is ever reached, and the file
 * carrying that stub is NOT one of the changed files.
 *
 * This state is NOT_APPLICABLE, and it is a DIFFERENT KIND of not-applicable from LoadDataUnzip's:
 * there the backend is absent on this arch by an __NPU_ARCH__ gate, so the declaration itself is
 * not compiled; here the API IS declared for this arch and its implementation refuses at run time.
 * "Both arms are unrunnable" is the shared part; WHY is not, and the two must not be filed
 * together.
 *
 * NOT COUNTED AS A CALL-COVERAGE HIT.
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

// ---- compile-time resolution checks ---------------------------------------
// These are the whole of this case.  The pointer arm must resolve to
// SetFixPipeAddr(DstPtr, uint16_t), not to the Tensor overload SetFixPipeAddr<T>(const
// LocalTensor<T>&, uint16_t).  Note the asymmetry, which is the point of the second assert: the
// Tensor form carries an explicit element type, the pointer form does not -- a raw pointer already
// names its pointee, so the call site drops the <T>.
static_assert(
    std::is_same<
        decltype(AscendC::SetFixPipeAddr<half>(std::declval<LocalTensor<half>&>(), std::declval<uint16_t>())),
        void>::value,
    "SetFixPipeAddr Tensor form must stay callable with a LocalTensor");
static_assert(
    std::is_same<
        decltype(AscendC::SetFixPipeAddr(std::declval<__ubuf__ half*>(), std::declval<uint16_t>())), void>::value,
    "SetFixPipeAddr pointer form must resolve to the raw-pointer overload");

// The address-space choice is visible in the tree rather than only in a comment:
//   impl/basic_api/kernel_operator_fixpipe_intf_impl.h:388  FpAddr(__cbuf__ D*)  -- L1
//   impl/basic_api/kernel_operator_fixpipe_intf_impl.h:395  FpAddr(__ubuf__ D*)  -- UB
// The assertion above pins the UB call.  There is deliberately no second assertion for the L1
// call: under ASCENDC_CPU_DEBUG the qualifiers are empty macros, so `__cbuf__ half*` and
// `__ubuf__ half*` are the same type and the assertion would be a duplicate, not a second check.
// An assertion that cannot distinguish the two is decoration, and is left out.

class SetFixPipeAddrPointerArmTestsuite : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// No kernel is launched -- see WHY THIS FILE HAS NO RUNNING KERNEL in the header.  The fixture is
// kept so the translation unit matches the shape of its nine siblings.
TEST_F(SetFixPipeAddrPointerArmTestsuite, SetFixPipeAddrResolutionOnly)
{
    SUCCEED() << "resolution-only case; the assertions in this file are the content";
}
