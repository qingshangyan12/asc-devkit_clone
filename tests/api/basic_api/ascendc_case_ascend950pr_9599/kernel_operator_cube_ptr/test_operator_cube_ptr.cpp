/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
/**
 * \file test_operator_cube_ptr.cpp
 * \brief Compile-time signature lock for the CUBE Basic API pointer overloads.
 *
 * Every pointer overload provided by
 *   impl/basic_api/kernel_operator_mm_ptr_impl.h
 *   impl/basic_api/kernel_operator_fixpipe_ptr_impl.h
 * is instantiated here exactly once, with the same call shape that the 14 samples under
 * examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/ use.  This file therefore fails to
 * compile if an overload is removed, renamed, re-ordered, or made ambiguous.
 *
 * It is deliberately a compile-only TU: no reference data is needed, so it can run on any
 * machine that has ccec.  See README.md for the exact command line.
 */
#include "kernel_operator.h"

using namespace AscendC;

namespace {
constexpr FixpipeConfig CFG_ROW_MAJOR_UB = {CO2Layout::ROW_MAJOR, true};
constexpr FixpipeConfig CFG_NZ_UB = {CO2Layout::NZ, true};
} // namespace

/* ------------------------------------------------------------------------------------------------
 * GetUnderlyingPtr: bare hardware pointer passes through, Tensor resolves through GetPhyAddr()
 * ---------------------------------------------------------------------------------------------- */
static_assert(Std::is_pointer_v<__gm__ half*>, "address-space qualified pointer must be a pointer");
static_assert(Std::is_pointer_v<__cc__ float*>, "address-space qualified pointer must be a pointer");
static_assert(isRawHwPtrV<__cbuf__ half*>, "isRawHwPtrV must detect a raw hw pointer");
static_assert(!isRawHwPtrV<LocalTensor<half>>, "LocalTensor must not be treated as a raw pointer");

/* ------------------------------------------------------------------------------------------------
 * LoadData family
 * ---------------------------------------------------------------------------------------------- */
template <typename U, typename Mx>
__aicore__ void ProbeLoadData(
    __ca__ U* a2, __cb__ U* b2, __cbuf__ U* a1, __cbuf__ Mx* mx, __gm__ U* gm, __ubuf__ U* ub,
    const LoadData2DParams& p2d, const LoadData2DParamsV2& p2dv2, const LoadData2DMxParams& pmx,
    const LoadData3DParamsV1<U>& p3dv1, const LoadData3DParamsV2<U>& p3dv2,
    const LoadData3DParamsV2Pro& p3dpro, const Load3DBitModeParam& p3bit,
    const Load2DBitModeParam& p2bit, const LoadData2dTransposeParams& ptr2d,
    const LoadData2dTransposeParamsV2& ptr2dv2)
{
    // L1 -> L0A / L0B
    LoadData(a2, a1, p2d);
    LoadData(b2, a1, p2d);
    LoadData(a2, a1, p2dv2);
    LoadData(b2, a1, p2dv2);
    // L1 -> L0A / L0B with MX scale
    LoadData(a2, a1, mx, p2dv2, pmx);
    LoadData(b2, a1, mx, p2dv2, pmx);
    // load3d v1 / v2, including the UB destination form
    LoadData(a2, a1, p3dv1);
    LoadData(b2, a1, p3dv1);
    LoadData(ub, a1, p3dv1);
    LoadData(a2, a1, p3dv2);
    LoadData(b2, a1, p3dv2);
    LoadData(ub, a1, p3dv2);
    // load3d v2 pro and the bit-mode parameter structs
    LoadData(a2, a1, p3dpro);
    LoadData(b2, a1, p3dpro);
    LoadData(a2, a1, p3bit);
    LoadData(b2, a1, p3bit);
    LoadData(a2, a1, p2bit);
    LoadData(b2, a1, p2bit);
    // GM -> L1 / L0A / L0B
    LoadData(a1, gm, p2dv2);
    LoadData(a2, gm, p2dv2);
    LoadData(b2, gm, p2dv2);
    // transpose
    LoadDataWithTranspose(a2, a1, ptr2d);
    LoadDataWithTranspose(b2, a1, ptr2d);
    LoadDataWithTranspose(b2, a1, ptr2dv2);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    LoadDataWithStride(a2, a1, p3dv2);
    LoadDataWithStride(b2, a1, p3dv2);
    LoadDataWithStride(ub, a1, p3dv2);
#endif
}

/* ------------------------------------------------------------------------------------------------
 * Mmad / MmadMx family
 * ---------------------------------------------------------------------------------------------- */
template <typename U>
__aicore__ void ProbeMmad(
    __cc__ U* cc, __ca__ U* a2, __cb__ U* b2, const MmadParams& pmmad, const MmadBitModeParams& pbit,
    MmadBitModeParams& pbitmut)
{
    Mmad(cc, a2, b2, pmmad);
    Mmad(cc, a2, b2, cc, pmmad); // bias passed as a bare __cc__ pointer (see README: cmatrixSource note)
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    Mmad(cc, a2, b2, pbit);
    Mmad(cc, a2, b2, cc, pbitmut);
    MmadMx(cc, a2, b2, pmmad);
    MmadMx(cc, a2, b2, cc, pmmad);
    MmadMx(cc, a2, b2, pbit);
    MmadMx(cc, a2, b2, cc, pbitmut);
#endif
}

/* ------------------------------------------------------------------------------------------------
 * Fixpipe / SetFixPipeConfig family
 * ---------------------------------------------------------------------------------------------- */
template <typename U>
__aicore__ void ProbeFixpipe(
    __cc__ U* cc, __cbuf__ U* cbuf, __ubuf__ U* ub, __gm__ U* gm, __cbuf__ uint64_t* ws,
    const FixpipeParamsV220& p220, const FixpipeParamsArch3510<CFG_NZ.format>& pnz,
    const FixpipeParamsArch3510<CFG_ROW_MAJOR.format>& prm,
    const FixpipeParamsArch3510<CFG_NZ_UB.format>& pnzub,
    const FixpipeParamsArch3510<CFG_ROW_MAJOR_UB.format>& prmub)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113 || \
                               __NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || \
                               __NPU_ARCH__ == 5163)
    Fixpipe(cbuf, cc, p220);
    Fixpipe(ub, cc, p220);
    Fixpipe(gm, cc, p220);
    Fixpipe(cbuf, cc, ws, p220);
    Fixpipe(ub, cc, ws, p220);
    Fixpipe(gm, cc, ws, p220);
#endif
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    Fixpipe<U, U, CFG_NZ>(cbuf, cc, pnz);
    Fixpipe<U, U, CFG_ROW_MAJOR>(cbuf, cc, prm);
    Fixpipe<U, U, CFG_NZ>(gm, cc, pnz);
    Fixpipe<U, U, CFG_ROW_MAJOR>(gm, cc, prm);
    Fixpipe<U, U, CFG_NZ_UB>(ub, cc, pnzub);
    Fixpipe<U, U, CFG_ROW_MAJOR_UB>(ub, cc, prmub);
    Fixpipe<U, U, CFG_NZ>(cbuf, cc, ws, pnz);
    Fixpipe<U, U, CFG_ROW_MAJOR>(gm, cc, ws, prm);
    Fixpipe<U, U, CFG_NZ_UB>(ub, cc, ws, pnzub);
#endif
    SetFixPipeConfig<U>(cc, true);
    SetFixPipeConfig<U>(cc, cc, true);
}

/* ------------------------------------------------------------------------------------------------
 * Sparse family (2201 only)
 * ---------------------------------------------------------------------------------------------- */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
template <typename U>
__aicore__ void ProbeSparse(__cc__ U* cc, __ca__ U* a2, __cb__ U* b2, __cbuf__ U* a1,
                            __cbuf__ uint8_t* idx, const MmadParams& pmmad, const LoadData2dParams& p2d)
{
    LoadDataWithSparse(b2, a1, idx, p2d);
    MmadWithSparse(cc, a2, b2, pmmad);
    MmadWithSparse(cc, a2, b2, {1, 2, 3, false, 0, false, false, false});
}
#endif

/* ------------------------------------------------------------------------------------------------
 * Entry: instantiate everything with concrete types
 * ---------------------------------------------------------------------------------------------- */
__aicore__ void ProbeCubePtrOverloads()
{
    using U = half;
    __ca__ U* a2 = (__ca__ U*)0;
    __cb__ U* b2 = (__cb__ U*)0;
    __cbuf__ U* a1 = (__cbuf__ U*)0;
    __gm__ U* gm = (__gm__ U*)0;
    __cc__ U* cc = (__cc__ U*)0;
    __ubuf__ U* ub = (__ubuf__ U*)0;
    __cbuf__ fp8_e8m0_t* mx = (__cbuf__ fp8_e8m0_t*)0;
    __cbuf__ uint64_t* ws = (__cbuf__ uint64_t*)0;

    LoadData2DParams p2d = {};
    LoadData2DParamsV2 p2dv2 = {};
    LoadData2DMxParams pmx = {};
    LoadData3DParamsV1<U> p3dv1 = {};
    LoadData3DParamsV2<U> p3dv2 = {};
    LoadData3DParamsV2Pro p3dpro = {};
    Load3DBitModeParam p3bit = {};
    Load2DBitModeParam p2bit = {};
    LoadData2dTransposeParams ptr2d = {};
    LoadData2dTransposeParamsV2 ptr2dv2 = {};
    MmadParams pmmad = {};
    MmadBitModeParams pbit = {};
    MmadBitModeParams pbitmut = {};
    FixpipeParamsV220 p220 = {};
    FixpipeParamsArch3510<CFG_NZ.format> pnz = {};
    FixpipeParamsArch3510<CFG_ROW_MAJOR.format> prm = {};
    FixpipeParamsArch3510<CFG_NZ_UB.format> pnzub = {};
    FixpipeParamsArch3510<CFG_ROW_MAJOR_UB.format> prmub = {};

    ProbeLoadData<U, fp8_e8m0_t>(a2, b2, a1, mx, gm, ub, p2d, p2dv2, pmx, p3dv1, p3dv2, p3dpro, p3bit,
                                 p2bit, ptr2d, ptr2dv2);
    ProbeMmad<U>(cc, a2, b2, pmmad, pbit, pbitmut);
    ProbeFixpipe<U>(cc, a1, ub, gm, ws, p220, pnz, prm, pnzub, prmub);
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    ProbeSparse<U>(cc, a2, b2, a1, (__cbuf__ uint8_t*)0, pmmad, p2d);
#endif
}
