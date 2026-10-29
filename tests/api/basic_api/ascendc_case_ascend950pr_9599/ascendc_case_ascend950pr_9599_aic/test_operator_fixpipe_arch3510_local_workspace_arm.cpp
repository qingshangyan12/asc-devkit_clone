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
 * \file test_operator_fixpipe_arch3510_local_workspace_arm.cpp
 * \brief Two-arm coverage for Fixpipe[16] (A4, the workspace form).
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
 *       error: no matching function for call to 'Fixpipe<half, int32_t, AscendC::CFG_ROW_MAJOR>(half*&, int*, long
 * unsigned int*, const AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR>&)' It is the SECOND static_assert
 * (the pointer form) that fails; the first (the Tensor form) is a control and still passes, which is what makes the
 * failure attributable to the pointer overload rather than to a broken file. A case that compiles on both trees would
 * be decorative; this one is not.
 *
 * COVERAGE CLAIM -- read this before citing the file
 *   Covered overloads (the join against the authoritative 25-name / 64-overload table is in
 *   the overload map captured together with the run):
 *     Fixpipe[16] -- Tensor form  : Fixpipe<T, U, config>(const LocalTensor<T>&, const LocalTensor<U>&, const
 * LocalTensor<uint64_t>& cbufWorkspace, const FixpipeParamsArch3510<format>&) Fixpipe[16] -- Pointer form : Fixpipe<T,
 * U, config, DstPtr, SrcPtr, WorkspacePtr>(DstPtr, SrcPtr, WorkspacePtr, const FixpipeParamsArch3510<format>&) (SFINAE
 * on CubePtrOverload::AnyPtr; quadrant key OV_Fixpipe_16_Arch3510_Local_workspace)
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES -- and this is the whole of it -- that the raw-pointer overload of Fixpipe[16] RESOLVES to a
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
constexpr int32_t FP_M = 32;
constexpr int32_t FP_N = 64;
constexpr int32_t FP_LEN = FP_M * FP_N;
constexpr int32_t SENTINEL = 0x5A5A1234;
} // namespace

// Fixpipe[16]: L0C -> L1, dav-3510 params, with a workspace
// (include/basic_api/kernel_operator_fixpipe_intf.h:150-152).

// ---- compile-time resolution checks ---------------------------------------
// Tensor arm: the LocalTensor/GlobalTensor form with the workspace third parameter.
static_assert(
    std::is_same<
        decltype(AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(
            std::declval<LocalTensor<half>&>(), std::declval<LocalTensor<int32_t>&>(),
            std::declval<LocalTensor<uint64_t>&>(),
            std::declval<const FixpipeParamsArch3510<CO2Layout::ROW_MAJOR>&>())),
        void>::value,
    "Fixpipe[16] Tensor+workspace form must resolve");
// Pointer arm: the raw-pointer form with the workspace third parameter.  Same config and same
// params, so the two arms differ only in how the operands are named.
static_assert(
    std::is_same<
        decltype(AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(
            std::declval<__cbuf__ half*&>(), std::declval<__cc__ int32_t*>(), std::declval<__cbuf__ uint64_t*>(),
            std::declval<const FixpipeParamsArch3510<CO2Layout::ROW_MAJOR>&>())),
        void>::value,
    "Fixpipe[16] pointer+workspace form must resolve");

class FixpipeArch3510LocalWorkspace : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// ARM T -- Tensor form.
__global__ __aicore__ void KernelFixpipeArch3510LocalWorkspaceTensorArm(__gm__ half* dstGm, __gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(dstL1, half, A1, 0, FP_LEN)
    LOCAL_TENSOR_REGISTER(co1, int32_t, CO1, 0, FP_LEN)
    LOCAL_TENSOR_REGISTER(cbufWorkspace, uint64_t, C1, 0, FP_LEN)

    FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> params = {
        FP_N, FP_M, static_cast<uint16_t>(AlignUp(FP_M, BLOCK_CUBE)), FP_N};
    params.params = {1, 0, 0};
    params.quantPre = QuantMode_t::VDEQF16;

    AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(dstL1, co1, cbufWorkspace, params);

    result[0] = SENTINEL;
}

// ARM P -- pointer form.  Same setup, same parameters, same assertion; only the operands are raw
// pointers.  A1 -> __cbuf__ (L1), CO1 -> __cc__ (L0C), the workspace -> __ubuf__ (UB).
__global__ __aicore__ void KernelFixpipeArch3510LocalWorkspacePointerArm(__gm__ half* dstGm, __gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(dstL1, half, A1, 0, FP_LEN)
    LOCAL_TENSOR_REGISTER(co1, int32_t, CO1, 0, FP_LEN)
    LOCAL_TENSOR_REGISTER(cbufWorkspace, uint64_t, C1, 0, FP_LEN)

    FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> params = {
        FP_N, FP_M, static_cast<uint16_t>(AlignUp(FP_M, BLOCK_CUBE)), FP_N};
    params.params = {1, 0, 0};
    params.quantPre = QuantMode_t::VDEQF16;

    AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(
        (__cbuf__ half*)dstL1.GetPhyAddr(), (__cc__ int32_t*)co1.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), params);

    result[0] = SENTINEL;
}

TEST_F(FixpipeArch3510LocalWorkspace, FixpipeArch3510LocalWorkspaceTensorArmRuns)
{
    static half dstGm[FP_LEN] = {0};
    static int32_t result[1] = {0};
    KernelFixpipeArch3510LocalWorkspaceTensorArm(dstGm, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Tensor-arm kernel body did not run to completion";
}

TEST_F(FixpipeArch3510LocalWorkspace, FixpipeArch3510LocalWorkspacePointerArmRuns)
{
    static half dstGm[FP_LEN] = {0};
    static int32_t result[1] = {0};
    KernelFixpipeArch3510LocalWorkspacePointerArm(dstGm, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Pointer-arm kernel body did not run to completion";
}
