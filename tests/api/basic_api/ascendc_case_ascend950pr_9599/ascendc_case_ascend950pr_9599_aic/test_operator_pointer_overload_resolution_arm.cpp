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
 * \file test_operator_pointer_overload_resolution_arm.cpp
 * \brief Resolution checks for four call shapes raised in review of the pointer overloads.
 *
 * Each assertion pins a call that the overload set previously either rejected or resolved to the
 * wrong overload.  They are compile-time only: no kernel is launched.
 *
 * Coverage boundary, stated so it is not over-read:
 *   - What these prove: the call is well-formed against the current declarations and the overload
 *     constraints admit it.  For SetFixPipeConfig(ptr, true) that is the whole question -- the
 *     two-operand overload now requires two pointers, so a bool second argument has nowhere else
 *     to bind and "this expression compiles" means "the bool is isUnitFlag".
 *   - What they do NOT prove: that the selected overload is the one intended, and -- for the
 *     LoadData GM -> L1 case -- that the device dispatch is correct.  Under ASCENDC_CPU_DEBUG the
 *     address-space qualifiers are empty macros, so `__cbuf__ half*` and `__gm__ half*` are the same
 *     type here and the assertion cannot distinguish the GM -> L1 helper from the L1 -> L0 ones.
 *     That path is exercised only by a device build.
 */
#include <type_traits>
#include <utility>

#include "gtest/gtest.h"
#include "kernel_operator.h"

using namespace AscendC;

// ---------------------------------------------------------------------------
// SetFixPipeConfig: the single-operand pointer form must be deducible.
//
// A plain "this call compiles" assertion has no power here: `SetFixPipeConfig(ptr, true)` compiled
// before the fix as well -- it silently bound the two-operand form with the bool as the quant
// address.  The assertion below therefore probes the second operand slot directly, through the
// three-argument shape so that arity cannot rescue it: if `B` may sit in the quant slot, the
// two-operand form is viable and the probe is true.
// ---------------------------------------------------------------------------
template <typename A, typename B, typename = void>
struct CanBindTwoPointerOperands : std::false_type {};
template <typename A, typename B>
struct CanBindTwoPointerOperands<
    A, B, std::void_t<decltype(AscendC::SetFixPipeConfig(std::declval<A>(), std::declval<B>(), true))>>
    : std::true_type {};

static_assert(
    !CanBindTwoPointerOperands<__cbuf__ half*, bool>::value,
    "a bool must not be bindable as the quant address");
static_assert(
    !CanBindTwoPointerOperands<__cbuf__ half*, int>::value,
    "an integer must not be bindable as the quant address");
static_assert(
    !CanBindTwoPointerOperands<__cbuf__ half*, uint64_t>::value,
    "a raw address value must not be bindable as the pointer's pointee either");
static_assert(
    CanBindTwoPointerOperands<__cbuf__ half*, __cbuf__ half*>::value,
    "two pointer operands must still bind the two-operand form");

static_assert(
    std::is_same<decltype(AscendC::SetFixPipeConfig(std::declval<__cbuf__ half*>(), true)), void>::value,
    "SetFixPipeConfig(ptr, bool) must be callable: the bool is isUnitFlag, not a second address");
static_assert(
    std::is_same<decltype(AscendC::SetFixPipeConfig<true>(std::declval<__cbuf__ half*>(), true)), void>::value,
    "SetFixPipeConfig<setRelu>(ptr, bool) must keep setRelu selectable");

// ---------------------------------------------------------------------------
// Fixpipe: explicit element types without an explicit config.
// `Fixpipe<half, float>(dst, src, params)` must pick up the CFG_ROW_MAJOR default, as the
// GlobalTensor / LocalTensor form does.
// ---------------------------------------------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::Fixpipe<half, float>(
            std::declval<__gm__ half*>(), std::declval<__cc__ float*>(), std::declval<FixpipeParamsV220>())),
        void>::value,
    "Fixpipe<T, U>(ptr, ptr, params) must compile with the default config");
static_assert(
    std::is_same<
        decltype(AscendC::Fixpipe<half, float>(
            std::declval<__gm__ half*>(), std::declval<__cc__ float*>(), std::declval<__cbuf__ uint64_t*>(),
            std::declval<FixpipeParamsV220>())),
        void>::value,
    "Fixpipe<T, U>(ptr, ptr, workspace, params) must compile with the default config");

// ---------------------------------------------------------------------------
// LoadData: GM -> L1 through the pointer entry points.
// The Ld12L0 helper had only L1-source forms, so an L1 destination fed from GM did not match.
// ---------------------------------------------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::LoadData(
            std::declval<__cbuf__ half*>(), std::declval<__gm__ half*>(), std::declval<LoadData2DParams>())),
        void>::value,
    "LoadData(L1 ptr, GM ptr, LoadData2DParams) must resolve: GM -> L1 is a supported direction");
static_assert(
    std::is_same<
        decltype(AscendC::LoadData(
            std::declval<__cbuf__ half*>(), std::declval<__gm__ half*>(), std::declval<LoadData2DParamsV2>())),
        void>::value,
    "LoadData(L1 ptr, GM ptr, LoadData2DParamsV2) must resolve for the V2 descriptor as well");

// ---------------------------------------------------------------------------
// MmadMx / Mmad BitMode: the descriptor is copied before it reaches the backend, so the
// interface must accept a const object or a temporary.
// ---------------------------------------------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::MmadMx(
            std::declval<__cc__ float*>(), std::declval<__ca__ half*>(), std::declval<__cb__ half*>(),
            std::declval<const MmadBitModeParams&>())),
        void>::value,
    "MmadMx BitMode pointer form must accept a const descriptor");
static_assert(
    std::is_same<
        decltype(AscendC::MmadMx(
            std::declval<__cc__ float*>(), std::declval<__ca__ half*>(), std::declval<__cb__ half*>(),
            std::declval<__cc__ float*>(), std::declval<const MmadBitModeParams&>())),
        void>::value,
    "MmadMx BitMode bias pointer form must accept a const descriptor");
static_assert(
    std::is_same<
        decltype(AscendC::Mmad(
            std::declval<__cc__ float*>(), std::declval<__ca__ half*>(), std::declval<__cb__ half*>(),
            std::declval<__cc__ float*>(), std::declval<const MmadBitModeParams&>())),
        void>::value,
    "Mmad BitMode bias pointer form must accept a const descriptor");

class PointerOverloadResolutionTestsuite : public testing::Test {
protected:
    void SetUp() { g_coreType = AIC_TYPE; }
    void TearDown() { g_coreType = MIX_TYPE; }
};

TEST_F(PointerOverloadResolutionTestsuite, ResolutionOnly)
{
    SUCCEED() << "resolution-only case; the static assertions are the content";
}
