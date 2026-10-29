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
 * \file test_operator_loadimagetolocal_pointer_arm.cpp
 * \brief Two-arm coverage for the raw-pointer overload of LoadImageToLocal (A4).
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
 *       error: no matching function for call to 'LoadImageToLocal(half*, const AscendC::LoadImageToLocalParams&)'
 *   It is the SECOND static_assert (the pointer form) that fails; the first (the Tensor form) is a
 *   control and still passes, which is what makes the failure attributable to the pointer overload
 *   rather than to a broken file.
 *   A case that compiles on both trees would be decorative; this one is not.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads (the join against the authoritative 25-name / 64-overload table is in
 *   the overload map captured together with the run):
 *     LoadImageToLocal -- Tensor form  : LoadImageToLocal<T>(const LocalTensor<T>&, const LoadImageToLocalParams&)
 *     LoadImageToLocal -- Pointer form : LoadImageToLocal(DstPtr, const LoadImageToLocalParams&)   [__cbuf__ L1]
 *                  (SFINAE on CubePtrOverload::AnyPtr; quadrant key OV_LoadImageToLocal_1_dst)
 *
 * NOTE ON HOW THIS CASE GOT ITS SECOND ARM BACK -- and on an earlier version of this note
 *
 *   The first version of this file ran both arms with AippInputFormat::YUV420SP_U8, and the TENSOR
 *   arm died with SIGSEGV (run_rc=139).  That was written up here as "the call arm cannot run".
 *   THE CONCLUSION WAS WRONG.  The step that showed it was cheap, and the transcript is in
 *   raw/loadimagetolocal_discriminate.txt:
 *
 *     K5    empty kernel, no API call               OK        <- negative control
 *     K6    + SetAippFunctions only                 OK        <- the fixture is harmless
 *     K3    Tensor  form, YUV420SP_U8, 16x16        SIGSEGV
 *     K3p   POINTER form, same parameters           SIGSEGV   <- identical, so it is NOT the
 *                                                                overload choice: both arms reach
 *                                                                the same backend
 *     K4a   Tensor  form, YUV420SP_U8, 1x1          SIGABRT
 *     K4b   Tensor  form, RGB888_U8,   16x16        OK        <- *** the path works ***
 *     K4c   Tensor  form, singleLineCopy            SIGABRT
 *     K4bp  POINTER form, RGB888_U8,   16x16        OK        <- *** and so does the pointer arm ***
 *
 *   ★ K4b is the decisive line.  A path that "is not supported by the simulation" does not pass for
 *   one format and fault for another.  The fault is a property of THAT CONFIGURATION, so the
 *   earlier statement is refuted and the correct one is narrower: the YUV420SP configuration this
 *   file first used is not runnable under the CPU simulation; RGB888_U8 is.
 *
 *   ⇒ This case runs BOTH arms, with RGB888_U8.  It is a full two-arm case.  The earlier
 *   classification was a fixture defect of mine that I reported as a property of the tree, and it
 *   is kept in this note rather than deleted because the way it was caught -- one variant per
 *   process, since a crash otherwise swallows every later case in the same binary -- is the
 *   reusable part.
 *
 *   The per-variant isolation matters: in a single run, K3's SIGSEGV killed the process and K4a,
 *   K4b, K4c and K4bp never executed.  A harness that reads "the binary crashed" learns nothing
 *   about which configuration crashed.
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES -- and this is the whole of it -- that the raw-pointer overload of LoadImageToLocal RESOLVES to a
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
constexpr int32_t LI_L1_LEN = 1024;
constexpr int32_t LI_SRC_LEN = 65536;
constexpr int32_t SENTINEL = 0x5A5A1234;
} // namespace

// ---- compile-time resolution checks ---------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::LoadImageToLocal<half>(
            std::declval<LocalTensor<half>&>(), std::declval<const LoadImageToLocalParams&>())),
        void>::value,
    "LoadImageToLocal Tensor form must stay callable with a LocalTensor destination");
static_assert(
    std::is_same<
        decltype(AscendC::LoadImageToLocal(
            std::declval<__cbuf__ half*>(), std::declval<const LoadImageToLocalParams&>())),
        void>::value,
    "LoadImageToLocal pointer form must resolve to the raw-pointer overload");

// The address space is pinned in the tree, not only in a comment:
//   impl/basic_api/kernel_operator_mm_intf_impl.h:946   LdImage(__cbuf__ D*)   <- the L1 form
// and LoadImageToLocalCal is reached from there.

class LoadImageToLocalTestsuite : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// ARM T -- Tensor form.  RGB888_U8 and a 16x16 image: see the header note for why this
// configuration and not YUV420SP.
__global__ __aicore__ void KernelLoadImageToLocalTensorArm(__gm__ uint8_t* srcGm, __gm__ int32_t* result)
{
    GlobalTensor<uint8_t> src;
    src.SetGlobalBuffer((__gm__ uint8_t*)srcGm, LI_SRC_LEN);
    AippParams<half> aipp{};
    AscendC::SetAippFunctions(src, AippInputFormat::RGB888_U8, aipp);

    LOCAL_TENSOR_REGISTER(dstL1, half, A1, 0, LI_L1_LEN)
    LoadImageToLocalParams params(16, 16, 0, 0, 16, 0, 0, 0, 0);

    AscendC::LoadImageToLocal<half>(dstL1, params);

    result[0] = SENTINEL;
}

// ARM P -- pointer form.  Same source, same AIPP configuration, same parameters, same assertion;
// only the destination is named as a raw L1 pointer.
__global__ __aicore__ void KernelLoadImageToLocalPointerArm(__gm__ uint8_t* srcGm, __gm__ int32_t* result)
{
    GlobalTensor<uint8_t> src;
    src.SetGlobalBuffer((__gm__ uint8_t*)srcGm, LI_SRC_LEN);
    AippParams<half> aipp{};
    AscendC::SetAippFunctions(src, AippInputFormat::RGB888_U8, aipp);

    LOCAL_TENSOR_REGISTER(dstL1, half, A1, 0, LI_L1_LEN)
    LoadImageToLocalParams params(16, 16, 0, 0, 16, 0, 0, 0, 0);

    AscendC::LoadImageToLocal((__cbuf__ half*)dstL1.GetPhyAddr(), params);

    result[0] = SENTINEL;
}

TEST_F(LoadImageToLocalTestsuite, LoadImageToLocalTensorArmRuns)
{
    static uint8_t srcGm[LI_SRC_LEN] = {0};
    static int32_t result[1] = {0};
    KernelLoadImageToLocalTensorArm(srcGm, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Tensor-arm kernel body did not run to completion";
}

TEST_F(LoadImageToLocalTestsuite, LoadImageToLocalPointerArmRuns)
{
    static uint8_t srcGm[LI_SRC_LEN] = {0};
    static int32_t result[1] = {0};
    KernelLoadImageToLocalPointerArm(srcGm, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Pointer-arm kernel body did not run to completion";
}
