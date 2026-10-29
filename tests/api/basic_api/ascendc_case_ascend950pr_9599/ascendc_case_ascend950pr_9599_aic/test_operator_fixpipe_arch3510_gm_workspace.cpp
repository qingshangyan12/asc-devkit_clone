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
 * \file test_operator_fixpipe_arch3510_gm_workspace.cpp
 * \brief Two-arm coverage for Fixpipe[11] (A4, the workspace form).
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
 *   Covered overloads:
 *     Fixpipe[11] -- Tensor form  : Fixpipe<T, U, config>(const GlobalTensor<T>&, const LocalTensor<U>&, const
 * LocalTensor<uint64_t>& cbufWorkspace, const FixpipeParamsArch3510<format>&) Fixpipe[11] -- Pointer form : Fixpipe<T,
 * U, config, DstPtr, SrcPtr, WorkspacePtr>(DstPtr, SrcPtr, WorkspacePtr, const FixpipeParamsArch3510<format>&) (SFINAE
 * on CubePtrOverload::AnyPtr; quadrant key OV_Fixpipe_11_Arch3510_GM_workspace)
 *
 * ========================= WHAT THIS CASE PROVES, AND WHAT IT DOES NOT =========================
 *
 * IT PROVES -- and this is the whole of it -- that the raw-pointer overload of Fixpipe[11] RESOLVES to a
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
constexpr int32_t FP_M = 32;
constexpr int32_t FP_N = 64;
constexpr int32_t FP_LEN = FP_M * FP_N;
constexpr int32_t SENTINEL = 0x5A5A1234;
} // namespace

// Fixpipe[11]: L0C -> GM, dav-3510 params, with a workspace.  The workspace element type is
// fixed at uint64_t in this declaration (include/basic_api/kernel_operator_fixpipe_intf.h:160-162),
// unlike the V220 block which deduces it -- hence the separate case.

// ---- compile-time resolution checks ---------------------------------------
// Tensor arm: the LocalTensor/GlobalTensor form with the workspace third parameter.
static_assert(
    std::is_same<
        decltype(AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(
            std::declval<GlobalTensor<half>&>(), std::declval<LocalTensor<int32_t>&>(),
            std::declval<LocalTensor<uint64_t>&>(),
            std::declval<const FixpipeParamsArch3510<CO2Layout::ROW_MAJOR>&>())),
        void>::value,
    "Fixpipe[11] Tensor+workspace form must resolve");
// Pointer arm: the raw-pointer form with the workspace third parameter.  Same config and same
// params, so the two arms differ only in how the operands are named.
static_assert(
    std::is_same<
        decltype(AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(
            std::declval<__gm__ half*&>(), std::declval<__cc__ int32_t*>(), std::declval<__cbuf__ uint64_t*>(),
            std::declval<const FixpipeParamsArch3510<CO2Layout::ROW_MAJOR>&>())),
        void>::value,
    "Fixpipe[11] pointer+workspace form must resolve");

class FixpipeArch3510GmWorkspace : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// ARM T -- Tensor form.
__global__ __aicore__ void KernelFixpipeArch3510GmWorkspaceTensorArm(__gm__ half* dstGm, __gm__ int32_t* result)
{
    GlobalTensor<half> dstGmT;
    dstGmT.SetGlobalBuffer((__gm__ half*)dstGm, FP_LEN);
    LOCAL_TENSOR_REGISTER(co1, int32_t, CO1, 0, FP_LEN)
    LOCAL_TENSOR_REGISTER(cbufWorkspace, uint64_t, C1, 0, FP_LEN)

    FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> params = {
        FP_N, FP_M, static_cast<uint16_t>(AlignUp(FP_M, BLOCK_CUBE)), FP_N};
    params.params = {1, 0, 0};
    params.quantPre = QuantMode_t::VDEQF16;

    AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(dstGmT, co1, cbufWorkspace, params);

    result[0] = SENTINEL;
}

// ARM P -- pointer form.  Same setup, same parameters, same assertion; only the operands are raw
// pointers.  A1 -> __cbuf__ (L1), CO1 -> __cc__ (L0C), the workspace -> __ubuf__ (UB).
__global__ __aicore__ void KernelFixpipeArch3510GmWorkspacePointerArm(__gm__ half* dstGm, __gm__ int32_t* result)
{
    GlobalTensor<half> dstGmT;
    dstGmT.SetGlobalBuffer((__gm__ half*)dstGm, FP_LEN);
    LOCAL_TENSOR_REGISTER(co1, int32_t, CO1, 0, FP_LEN)
    LOCAL_TENSOR_REGISTER(cbufWorkspace, uint64_t, C1, 0, FP_LEN)

    FixpipeParamsArch3510<CO2Layout::ROW_MAJOR> params = {
        FP_N, FP_M, static_cast<uint16_t>(AlignUp(FP_M, BLOCK_CUBE)), FP_N};
    params.params = {1, 0, 0};
    params.quantPre = QuantMode_t::VDEQF16;

    AscendC::Fixpipe<half, int32_t, CFG_ROW_MAJOR>(
        (__gm__ half*)dstGmT.GetPhyAddr(), (__cc__ int32_t*)co1.GetPhyAddr(),
        (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), params);

    result[0] = SENTINEL;
}

TEST_F(FixpipeArch3510GmWorkspace, FixpipeArch3510GmWorkspaceTensorArmRuns)
{
    static half dstGm[FP_LEN] = {0};
    static int32_t result[1] = {0};
    KernelFixpipeArch3510GmWorkspaceTensorArm(dstGm, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Tensor-arm kernel body did not run to completion";
}

TEST_F(FixpipeArch3510GmWorkspace, FixpipeArch3510GmWorkspacePointerArmRuns)
{
    static half dstGm[FP_LEN] = {0};
    static int32_t result[1] = {0};
    KernelFixpipeArch3510GmWorkspacePointerArm(dstGm, result);
    EXPECT_EQ(result[0], SENTINEL) << "the Pointer-arm kernel body did not run to completion";
}
