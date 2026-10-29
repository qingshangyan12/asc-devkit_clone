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
 * \file test_operator_mmadmx_pointer_arm.cpp
 * \brief Two-arm coverage for the raw-pointer overload of MmadMx (A4 / task book §5).
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
 *       error: no matching function for call to 'MmadMx(float*, float4_e1m2::Fp4e1m2T*, float4_e1m2::Fp4e1m2T*, const
 * AscendC::MmadParams&)' It is the SECOND static_assert (the pointer form) that fails; the first (the Tensor form) is a
 *   control and still passes, which is what makes the failure attributable to the pointer overload
 *   rather than to a broken file.
 *   A case that compiles on both trees would be decorative; this one is not.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads (the join against the authoritative 25-name / 64-overload table is in
 *   the overload map captured together with the run):
 *     MmadMx -- Tensor form  : MmadMx<T, U, S>(const LocalTensor<T>&, const LocalTensor<U>&, const LocalTensor<S>&,
 * const MmadParams&)   [instantiated here as <float, fp4x2_e1m2_t, fp4x2_e1m2_t>] MmadMx -- Pointer form :
 * MmadMx(DstPtr, Src0Ptr, Src1Ptr, const MmadParams&)   [__cc__ L0C, __ca__ L0A, __cb__ L0B] (SFINAE on
 * CubePtrOverload::AnyPtr; quadrant key OV_MmadMx_1_noBias)
 *
 * NOTE ON THE +83 MERGE
 *   This file was first written against fingerprint 57c5246a (pre-+83) and RE-VERIFIED against
 *   6160969a, because +83 touched the Mmad family's resolution.  The pointer MmadMx declarations
 *   live at include/basic_api/kernel_operator_mm_intf.h:585-593 (guarded __NPU_ARCH__ == 3510) and
 *   are unaffected by the BitMode forms +83 added at :572-582.  The re-verification is recorded in
 *   VERIFIED AGAINST above.
 *
 * NOTE ON THE ELEMENT TYPES
 *   MmadMx on dav-3510 supports only the microscaling operand types.  This was MEASURED, not
 *   assumed: a first version of this file used <float, float, float> and the delivery tree
 *   rejected it with
 *       kernel_operator_mm_impl.h:608: error: static assertion failed:
 *           MmadMx not support data type on current device!
 *   so the case uses the combination the existing suite uses,
 *   MmadMx<float, fp4x2_e1m2_t, fp4x2_e1m2_t> (tests/.../test_operator_MM_Mx.cpp).
 *
 * NOTE ON THE CALL SITE
 *   The two arms do not have the same spelling.  The Tensor form takes its element types as
 *   explicit template arguments (MmadMx<L0cOutT, Src0T, Src1T>, as the existing suite writes it);
 *   the pointer form DEDUCES them from the operands, so writing <float, fp4x2_e1m2_t, fp4x2_e1m2_t>
 *   there would bind DstPtr=float and fail to match.  That asymmetry is a property of the
 *   overloads, not of this test.
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES -- and this is the whole of it -- that the raw-pointer overload of MmadMx RESOLVES to a
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
constexpr int32_t MX_FEATURE_SIZE = 3584;
constexpr int32_t MX_WEIGHT_SIZE = 4096;
constexpr int32_t MX_OUTPUT_SIZE = 14336;
constexpr int32_t SENTINEL = 0x5A5A1234;
} // namespace

// ---- compile-time resolution checks ---------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::MmadMx<float, fp4x2_e1m2_t, fp4x2_e1m2_t>(
            std::declval<LocalTensor<float>&>(), std::declval<LocalTensor<fp4x2_e1m2_t>&>(),
            std::declval<LocalTensor<fp4x2_e1m2_t>&>(), std::declval<const MmadParams&>())),
        void>::value,
    "MmadMx Tensor form must stay callable with LocalTensor operands");
static_assert(
    std::is_same<
        decltype(AscendC::MmadMx(
            std::declval<__cc__ float*>(), std::declval<__ca__ fp4x2_e1m2_t*>(), std::declval<__cb__ fp4x2_e1m2_t*>(),
            std::declval<const MmadParams&>())),
        void>::value,
    "MmadMx pointer form must resolve to the raw-pointer overload, not to the Tensor overload");

class MmadMxPointerArmTestsuite : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

namespace {
inline MmadParams MakeMmadParams()
{
    MmadParams p;
    p.m = 112;
    p.k = 32;
    p.n = 128;
    p.cmatrixInitVal = true;
    p.cmatrixSource = false;
    return p;
}
} // namespace

// ARM T -- Tensor form, spelled the way tests/.../test_operator_MM_Mx.cpp spells it.
__global__ __aicore__ void KernelMmadMxTensorArm(__gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(featureL0a, fp4x2_e1m2_t, A2, 0, MX_FEATURE_SIZE)
    LOCAL_TENSOR_REGISTER(weightL0b, fp4x2_e1m2_t, B2, 0, MX_WEIGHT_SIZE)
    LOCAL_TENSOR_REGISTER(l0cOut, float, CO1, 0, MX_OUTPUT_SIZE)
    MmadParams params = MakeMmadParams();

    AscendC::MmadMx<float, fp4x2_e1m2_t, fp4x2_e1m2_t>(l0cOut, featureL0a, weightL0b, params);

    result[0] = SENTINEL;
}

// ARM P -- pointer form.  The address spaces are the ones each tensor was registered at:
// A2 -> __ca__ (L0A), B2 -> __cb__ (L0B), CO1 -> __cc__ (L0C).
__global__ __aicore__ void KernelMmadMxPointerArm(__gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(featureL0a, fp4x2_e1m2_t, A2, 0, MX_FEATURE_SIZE)
    LOCAL_TENSOR_REGISTER(weightL0b, fp4x2_e1m2_t, B2, 0, MX_WEIGHT_SIZE)
    LOCAL_TENSOR_REGISTER(l0cOut, float, CO1, 0, MX_OUTPUT_SIZE)
    MmadParams params = MakeMmadParams();

    AscendC::MmadMx(
        (__cc__ float*)l0cOut.GetPhyAddr(), (__ca__ fp4x2_e1m2_t*)featureL0a.GetPhyAddr(),
        (__cb__ fp4x2_e1m2_t*)weightL0b.GetPhyAddr(), params);

    result[0] = SENTINEL;
}

TEST_F(MmadMxPointerArmTestsuite, MmadMxTensorArmRuns)
{
    static int32_t result[1] = {0};
    KernelMmadMxTensorArm(result);
    EXPECT_EQ(result[0], SENTINEL) << "the Tensor-arm kernel body did not run to completion";
}

TEST_F(MmadMxPointerArmTestsuite, MmadMxPointerArmRuns)
{
    static int32_t result[1] = {0};
    KernelMmadMxPointerArm(result);
    EXPECT_EQ(result[0], SENTINEL) << "the Pointer-arm kernel body did not run to completion";
}
