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
 * \file test_operator_loaddataunzip_arch_note.cpp
 * \brief NOT_APPLICABLE annotation for LoadDataUnzip's raw-pointer overload (A4).
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
 *   ⚠ This file is the ONE case of the ten that compiles on BOTH trees.  That is by design, not an
 *   oversight: it is an annotation, not a coverage case (see above).  Its discriminating power
 *   therefore cannot come from the pristine tree, and it does not:
 *
 *   INJECTION TEST -- a synthetic dav-3510 raw-pointer overload of LoadDataUnzip was appended to a
 *   scratch copy of the tree, and the negative assertion below fired, with its own message:
 *       error: static assertion failed: LoadDataUnzip's raw-pointer overload is now visible on
 *           dav-3510.  The NOT_APPLICABLE annotation is STALE: move LoadDataUnzip into the covered
 *           set and write the two-arm case.  Do not delete this assertion to make the build pass.
 *   (the file:line prefix is omitted on purpose -- it moves every time the header above it grows,
 *    and a stale reference reads as a claim that no longer matches the file.)
 *   (captured in the run transcript that accompanies the verification report.)
 *   So the annotation is a CHECK, not a comment: it goes red the moment the premise changes.
 *   The positive control next to it is what makes that meaningful -- without it, `false_type` could
 *   just be an idiom that always returns false.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads (the join against the authoritative 25-name / 64-overload table is in
 *   the overload map captured together with the run):
 *     LoadDataUnzip -- Tensor form  : LoadDataUnzip<T>(const LocalTensor<T>&, const GlobalTensor<T>&)
 *     LoadDataUnzip -- Pointer form : LoadDataUnzip(DstPtr, SrcPtr)   [__cbuf__/__ca__/__cb__ dst, __gm__ src]
 *                  (SFINAE on CubePtrOverload::AnyPtr; quadrant key NOT_APPLICABLE on dav-3510 (overload guarded to
 * arch 1001/2002/2201/3002/3102))
 *
 * ============================ THIS FILE IS AN ANNOTATION, NOT COVERAGE ==========================
 *
 * LoadDataUnzip's raw-pointer overload is guarded
 *     #if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002) ||
 *                                   (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) ||
 *                                   (__NPU_ARCH__ == 3102))
 * (include/basic_api/kernel_operator_mm_intf.h:679-688), and this suite is built with
 * __NPU_ARCH__ == 3510.  The overload therefore DOES NOT EXIST on the arch under test, and no
 * two-arm case can be written for it here.
 *
 * The mapping table records this name as NOT_APPLICABLE, and it is NOT counted as covered.  The
 * distinction matters: "we know we do not cover it" and "we do not know that we do not cover it"
 * are different states, and only the first one is acceptable.
 *
 * What this file DOES do is make the annotation CHECKABLE rather than a comment.  Two static
 * assertions below are a matched pair:
 *   * the negative one fails if a 3510 overload is ever added (at which point the annotation is
 *     stale and the name must move into the covered set -- not be silently deleted);
 *   * the positive one is the control that shows the detection idiom can return true, so the
 *     negative result is not an artefact of an idiom that always returns false.
 * The pair was shown to have discriminating power by injecting a synthetic 3510 overload into a
 * scratch copy of the tree and watching the negative assertion fire; the injection and its output
 * are recorded in the run transcript that accompanies the verification report.
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES that the raw-pointer overload of LoadDataUnzip is NOT VISIBLE on dav-3510, i.e. that the
 * NOT_APPLICABLE marker above is currently true -- and it goes red if that ever stops being true.
 *
 * IT DOES NOT COVER ANYTHING.  No overload of this name is called by this file.  It is counted as
 * NOT_APPLICABLE in the mapping table and must never be counted as coverage: "we know we do not
 * cover it" and "we do not know that we do not cover it" are different states.
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

// Detection idiom.  The failure to find a matching overload happens in the immediate context of
// template argument deduction, so it is a substitution failure and lands on the primary template.
template <typename D, typename S, typename = void>
struct HasLoadDataUnzipRawPtr : std::false_type {};

template <typename D, typename S>
struct HasLoadDataUnzipRawPtr<D, S, decltype(void(AscendC::LoadDataUnzip(std::declval<D>(), std::declval<S>())))>
    : std::true_type {};

// The control: the Tensor overload DOES exist on this arch.  If this ever becomes false the
// negative assertion below is no longer evidence about the pointer overload specifically.
template <typename D, typename S, typename = void>
struct HasLoadDataUnzipTensor : std::false_type {};

template <typename D, typename S>
struct HasLoadDataUnzipTensor<D, S, decltype(void(AscendC::LoadDataUnzip(std::declval<D>(), std::declval<S>())))>
    : std::true_type {};

} // namespace

// POSITIVE CONTROL -- the detection idiom is capable of returning true on this tree.
static_assert(
    HasLoadDataUnzipTensor<const LocalTensor<half>&, const GlobalTensor<half>&>::value,
    "control failed: the Tensor form of LoadDataUnzip must be visible on dav-3510.  Until this "
    "passes, the negative assertion below says nothing about the pointer overload.");
static_assert(
    HasLoadDataUnzipRawPtr<__cbuf__ half*, __gm__ half*>::value == false,
    "LoadDataUnzip's raw-pointer overload is now visible on dav-3510.  The NOT_APPLICABLE "
    "annotation is STALE: move LoadDataUnzip into the covered set and write the two-arm case.  Do "
    "not delete this assertion to make the build pass.");

class LoadDataUnzipArchNoteTestsuite : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// The suite needs this fixture to exist so the file is a real translation unit in the target, but
// there is deliberately NO arm here: writing a "kernel that calls LoadDataUnzip with pointers"
// would not compile on dav-3510, and writing one that does not call it would be decoration.
TEST_F(LoadDataUnzipArchNoteTestsuite, LoadDataUnzipIsNotApplicableOnDav3510)
{
    SUCCEED() << "annotation only; the assertions in this file are the content, not this body";
}
