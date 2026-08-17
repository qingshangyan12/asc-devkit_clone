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
 * \file bessel_i0_3510_impl.h
 * \brief Kernel-side RegBase VF implementation of the BesselI0 high-level API for DAV_3510
 * (Ascend 950PR/Ascend 950DT).
 *
 * Algorithm: piecewise Chebyshev polynomial approximation, identical to PyTorch ATen
 * aten/src/ATen/native/Math.h::calc_i0 (Cephes i0/i0e coefficients):
 *   - Branch A (|x| <= 8):  I0(x) = Exp(ax) * chbevl(ax/2 - 2, A[30])
 *   - Branch B (|x| > 8):   I0(x) = Exp(ax - 0.5*Ln(ax) + Ln(chbevl(32/ax - 2, B[25])))
 *     (exponentially-scaled form; the Chebyshev correction term polyB is folded into the
 *     single Exp so that no intermediate value overflows prematurely. A naive evaluation of
 *     polyB * Exp(ax - 0.5*Ln(ax)) would overflow around |x| ~= 90.98, while the true float32
 *     overflow boundary of I0(x) is approximately |x| ~= 91.9.)
 *   - Final: result = Max(result, 1.0). I0(x) >= 1 mathematically; without this clamp, tiny
 *     inputs (|x| < ~7e-5, including x = +-0) may produce a value 1-3 ULP below 1.0. The clamp
 *     also guarantees an exact 1.0 (0x3f800000) at x = +-0, so no dedicated isZero branch is
 *     needed (bitwise-verified against the variant with an isZero special case).
 *   - Inputs with |x| > 200 (including +-Inf) are clamped to 200.0 before the B branch so that
 *     Ln/exponent never see an indeterminate form; the result then naturally overflows to +Inf.
 *
 * Implementation notes:
 *   1. The Clenshaw recurrences are fully unrolled (no array-driven for loop): indexing a
 *      global constexpr coefficient array with a runtime index inside a loop, combined with
 *      the cross-iteration register rotation b2=b1/b1=b0, fails to compile in the RegBase VF
 *      backend ("error in backend: Unsupported Inst must be hoisted"). The unrolled form
 *      compiles and passes precision validation.
 *   2. CMPMODE lives in namespace AscendC (not AscendC::Reg).
 *   3. oneRepeatSize is obtained from AscendC::GetVecLen() / sizeof(T) (64 for float32),
 *      not hard-coded.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/bessel_i0.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_BESSEL_I0_C310_IMPL_H__
#endif

#ifndef IMPL_MATH_BESSEL_I0_BESSEL_I0_C310_IMPL_H
#define IMPL_MATH_BESSEL_I0_BESSEL_I0_C310_IMPL_H

#include "kernel_operator.h"

namespace AscendC {

/*!
 * \brief Chebyshev coefficients of branch A (30 terms, x in [0, 8]), identical to PyTorch ATen
 * chebyshev_coefficients_i0e_A. array[0] is the highest-order term; the Clenshaw recurrence
 * processes the array in order.
 *
 * [[maybe_unused]]: this array is only kept as the literal cross-check reference for the fully
 * unrolled BesselI0Chbevl30 code below and is not referenced by device code (an array-driven
 * loop does not compile in this environment, see the file-level note).
 */
[[maybe_unused]] constexpr float BESSEL_I0_CHEBY_A[30] = {
    -4.41534164647933937950E-18f, 3.33079451882223809783E-17f,  -2.43127984654795469359E-16f,
    1.71539128555513303061E-15f,  -1.16853328779934516808E-14f, 7.67618549860493561688E-14f,
    -4.85644678311192946090E-13f, 2.95505266312963983461E-12f,  -1.72682629144155570723E-11f,
    9.67580903537323691224E-11f,  -5.18979560163526290666E-10f, 2.65982372468238665035E-9f,
    -1.30002500998624804212E-8f,  6.04699502254191894932E-8f,   -2.67079385394061173391E-7f,
    1.11738753912010371815E-6f,   -4.41673835845875056359E-6f,  1.64484480707288970893E-5f,
    -5.75419501008210370398E-5f,  1.88502885095841655729E-4f,   -5.76375574538582365885E-4f,
    1.63947561694133579842E-3f,   -4.32430999505057594430E-3f,  1.05464603945949983183E-2f,
    -2.37374148058994688156E-2f,  4.93052842396707084878E-2f,   -9.49010970480476444210E-2f,
    1.71620901522208775349E-1f,   -3.04682672343198398683E-1f,  6.76795274409476084995E-1f};

/*!
 * \brief Chebyshev coefficients of branch B (25 terms, x in (8, +inf)), identical to PyTorch ATen
 * chebyshev_coefficients_i0e_B (classic Cephes i0e.c double-precision coefficients).
 * array[0] is the highest-order term.
 *
 * [[maybe_unused]]: same as BESSEL_I0_CHEBY_A, kept only as the literal cross-check reference
 * for the fully unrolled BesselI0Chbevl25 code below.
 */
[[maybe_unused]] constexpr float BESSEL_I0_CHEBY_B[25] = {
    -7.23318048787475395456E-18f, -4.83050448594418207126E-18f, 4.46562142029675999901E-17f,
    3.46122286769746109310E-17f,  -2.82762398051658348494E-16f, -3.42548561967721913462E-16f,
    1.77256013305652638360E-15f,  3.81168066935262242075E-15f,  -9.55484669882830764870E-15f,
    -4.15056934728722208663E-14f, 1.54008621752140982691E-14f,  3.85277838274214270114E-13f,
    7.18012445138366623367E-13f,  -1.79417853150680611778E-12f, -1.32158118404477131188E-11f,
    -3.14991652796324136454E-11f, 1.18891471078464383424E-11f,  4.94060238822496958910E-10f,
    3.39623202570838634515E-9f,   2.26666899049817806459E-8f,   2.04891858946906374183E-7f,
    2.89137052083475648297E-6f,   6.88975834691682398426E-5f,   3.36911647825569408990E-3f,
    8.04490411014108831608E-1f};

/*!
 * \brief VF-safe Clenshaw recurrence (chbevl) evaluating the branch-A coefficients (30 terms).
 *
 * Recurrence:
 *     b0 = C[0]; b1 = 0; b2 = 0
 *     for i in 1..len-1: b2 = b1; b1 = b0; b0 = y*b1 - b2 + C[i]
 *     return 0.5 * (b0 - b2)
 *
 * The 29 steps are fully unrolled (no runtime array indexing) because the array-driven loop
 * form does not compile in the RegBase VF backend (see the file-level note); every literal
 * below matches BESSEL_I0_CHEBY_A[1..29] exactly.
 *
 * Only called inside a __simd_vf__ VF body; all operands stay in RegTensor (register domain),
 * with no UB access.
 */
template <typename T>
__simd_callee__ inline void BesselI0Chbevl30(
    AscendC::Reg::RegTensor<T>& dst, AscendC::Reg::RegTensor<T>& y, AscendC::Reg::MaskReg& mask)
{
    using namespace AscendC::Reg;

    RegTensor<T> b0, b1, b2, tmp;
    Duplicate<T>(b0, static_cast<T>(-4.41534164647933937950E-18));
    Duplicate<T>(b1, static_cast<T>(0.0));
    Duplicate<T>(b2, static_cast<T>(0.0));

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(3.33079451882223809783E-17), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-2.43127984654795469359E-16), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.71539128555513303061E-15), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-1.16853328779934516808E-14), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(7.67618549860493561688E-14), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-4.85644678311192946090E-13), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(2.95505266312963983461E-12), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-1.72682629144155570723E-11), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(9.67580903537323691224E-11), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-5.18979560163526290666E-10), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(2.65982372468238665035E-9), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-1.30002500998624804212E-8), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(6.04699502254191894932E-8), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-2.67079385394061173391E-7), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.11738753912010371815E-6), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-4.41673835845875056359E-6), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.64484480707288970893E-5), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-5.75419501008210370398E-5), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.88502885095841655729E-4), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-5.76375574538582365885E-4), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.63947561694133579842E-3), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-4.32430999505057594430E-3), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.05464603945949983183E-2), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-2.37374148058994688156E-2), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(4.93052842396707084878E-2), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-9.49010970480476444210E-2), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.71620901522208775349E-1), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-3.04682672343198398683E-1), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(6.76795274409476084995E-1), mask);

    Sub<T>(dst, b0, b2, mask);
    Muls<T>(dst, dst, static_cast<T>(0.5), mask);
}

/*!
 * \brief VF-safe Clenshaw recurrence (chbevl) evaluating the branch-B coefficients (25 terms).
 *
 * Same algorithm as BesselI0Chbevl30, with the coefficient table BESSEL_I0_CHEBY_B[25].
 * The 24 steps are fully unrolled for the same reason; every literal below matches
 * BESSEL_I0_CHEBY_B[1..24] exactly.
 *
 * Only called inside a __simd_vf__ VF body; all operands stay in RegTensor (register domain),
 * with no UB access.
 */
template <typename T>
__simd_callee__ inline void BesselI0Chbevl25(
    AscendC::Reg::RegTensor<T>& dst, AscendC::Reg::RegTensor<T>& y, AscendC::Reg::MaskReg& mask)
{
    using namespace AscendC::Reg;

    RegTensor<T> b0, b1, b2, tmp;
    Duplicate<T>(b0, static_cast<T>(-7.23318048787475395456E-18));
    Duplicate<T>(b1, static_cast<T>(0.0));
    Duplicate<T>(b2, static_cast<T>(0.0));

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-4.83050448594418207126E-18), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(4.46562142029675999901E-17), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(3.46122286769746109310E-17), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-2.82762398051658348494E-16), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-3.42548561967721913462E-16), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.77256013305652638360E-15), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(3.81168066935262242075E-15), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-9.55484669882830764870E-15), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-4.15056934728722208663E-14), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.54008621752140982691E-14), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(3.85277838274214270114E-13), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(7.18012445138366623367E-13), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-1.79417853150680611778E-12), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-1.32158118404477131188E-11), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(-3.14991652796324136454E-11), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(1.18891471078464383424E-11), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(4.94060238822496958910E-10), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(3.39623202570838634515E-9), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(2.26666899049817806459E-8), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(2.04891858946906374183E-7), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(2.89137052083475648297E-6), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(6.88975834691682398426E-5), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(3.36911647825569408990E-3), mask);

    b2 = b1; b1 = b0;
    Mul<T>(tmp, y, b1, mask); Sub<T>(tmp, tmp, b2, mask);
    Adds<T>(b0, tmp, static_cast<T>(8.04490411014108831608E-1), mask);

    Sub<T>(dst, b0, b2, mask);
    Muls<T>(dst, dst, static_cast<T>(0.5), mask);
}

/*!
 * \brief BesselI0 VF main body: two-state boundary masks + A/B branches + single Select +
 * lower-bound clamp.
 *
 * Branch structure:
 *   isSmall : ax <= 8.0 (branch A; x = +-0 naturally falls into this path)
 *             -> Exp(axSmall) * chbevl(axSmall/2 - 2, A[30])
 *   otherwise (branch B; isHuge clamps to 200.0 and the result then naturally overflows
 *             to +Inf):
 *             Exp(axLarge - 0.5*Ln(axLarge) + Ln(chbevl(32/axLarge - 2, B[25])))
 *   final   : Max(result, 1.0) -> removes the 1-3 ULP negative deviation from the
 *             I0(x) >= 1 invariant in the tiny-input region; at x = +-0 the raw branch-A
 *             value is 1 ULP below 1.0 and is pulled back to exactly 1.0 (0x3f800000).
 *
 * \param dstAddr UB destination address.
 * \param srcAddr UB source address.
 * \param count remaining number of elements to process (decremented inside UpdateMask on every
 *     call; passed by value here and re-read per block iteration).
 * \param oneRepeatSize number of elements processed per VF iteration (VL / sizeof(T), computed
 *     by the kernel-side BesselI0Impl and passed in).
 * \param repeatTimes number of blocks.
 */
template <typename T>
__simd_vf__ inline void BesselI0Vf(
    __ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count, uint32_t oneRepeatSize, uint32_t repeatTimes)
{
    using namespace AscendC::Reg;

    // Constant registers: materialized once per VF scope and reused inside the loop,
    // so no UB constant pool is needed.
    RegTensor<T> c8, c200, c1, c32;
    Duplicate<T>(c8, static_cast<T>(8.0));
    Duplicate<T>(c200, static_cast<T>(200.0));
    Duplicate<T>(c1, static_cast<T>(1.0));
    Duplicate<T>(c32, static_cast<T>(32.0));

    for (uint32_t i = 0; i < repeatTimes; ++i) {
        MaskReg mask = UpdateMask<T>(count);

        RegTensor<T> x, ax;
        LoadAlign<T>(x, srcAddr + i * oneRepeatSize);
        Abs<T>(ax, x, mask);

        // ---- two-state boundary masks (x = 0 naturally falls into the isSmall branch-A domain)
        MaskReg isSmall;
        MaskReg isHuge;
        Compares<T, AscendC::CMPMODE::LE>(isSmall, ax, static_cast<T>(8.0), mask);
        Compares<T, AscendC::CMPMODE::GT>(isHuge, ax, static_cast<T>(200.0), mask);

        // axSmall = Select(isSmall, ax, 8.0): lanes with isSmall=false are clamped to the safe
        // placeholder 8.0 (their results are discarded by the final Select)
        RegTensor<T> axSmall;
        Select<T>(axSmall, ax, c8, isSmall);

        // axLarge = Select(isSmall, 8.0, Select(isHuge, 200.0, ax)):
        //   lanes with isSmall=true are clamped to the safe placeholder 8.0 (discarded later);
        //   lanes with isHuge=true (ax > 200 or literal +-Inf) are clamped to 200.0 so that
        //   Ln/exponent never see an indeterminate form.
        RegTensor<T> axLargeInner, axLarge;
        Select<T>(axLargeInner, c200, ax, isHuge);
        Select<T>(axLarge, c8, axLargeInner, isSmall);

        // ---- branch A: resultSmall = Exp(axSmall) * chbevl(axSmall/2 - 2, A[30]) ----
        RegTensor<T> ySmall;
        Muls<T>(ySmall, axSmall, static_cast<T>(0.5), mask);
        Adds<T>(ySmall, ySmall, static_cast<T>(-2.0), mask);

        RegTensor<T> polyA;
        BesselI0Chbevl30<T>(polyA, ySmall, mask);

        RegTensor<T> expSmall, resultSmall;
        Exp<T>(expSmall, axSmall, mask);
        Mul<T>(resultSmall, expSmall, polyA, mask);

        // ---- branch B: resultLarge = Exp(axLarge - 0.5*Ln(axLarge) + Ln(polyB)) ----
        // Folding the Chebyshev correction term polyB into the single Exp prevents the
        // intermediate Exp(axLarge - 0.5*Ln(axLarge)) from overflowing prematurely around
        // x ~= 90.98; the overflow boundary now matches the true float32 overflow boundary
        // of I0(x) (approximately x ~= 91.9).
        RegTensor<T> yLarge;
        Div<T>(yLarge, c32, axLarge, mask);
        Adds<T>(yLarge, yLarge, static_cast<T>(-2.0), mask);

        RegTensor<T> polyB;
        BesselI0Chbevl25<T>(polyB, yLarge, mask);

        RegTensor<T> lnAxLarge, halfLn, exponent, lnPolyB, exponentStable, resultLarge;
        Ln<T>(lnAxLarge, axLarge, mask);
        Muls<T>(halfLn, lnAxLarge, static_cast<T>(0.5), mask);
        Sub<T>(exponent, axLarge, halfLn, mask);
        Ln<T>(lnPolyB, polyB, mask);
        Add<T>(exponentStable, exponent, lnPolyB, mask);
        Exp<T>(resultLarge, exponentStable, mask);

        // ---- final select and lower-bound clamp: single Select followed by Max(result, 1.0)
        RegTensor<T> resultBranch, resultClamped;
        Select<T>(resultBranch, resultSmall, resultLarge, isSmall);
        Max<T>(resultClamped, resultBranch, c1, mask);

        StoreAlign<T>(dstAddr + i * oneRepeatSize, resultClamped, mask);
    }
}

/*!
 * \brief BesselI0 kernel-side entry (non-VF): computes oneRepeatSize/repeatTimes and launches
 * the VF sub-task via asc_vf_call.
 *
 * oneRepeatSize = AscendC::GetVecLen() / sizeof(T), following the official asc_vf_call usage
 * examples. On DAV_3510 (Ascend 950PR/950DT) VL = 256B, so oneRepeatSize = 64 for float.
 * It is deliberately not hard-coded to avoid binding to a specific architecture width literal.
 */
template <typename T, bool isReuseSource>
__aicore__ inline void BesselI0Impl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    static_assert(SupportType<T, float>(), "current data type is not supported on current device!");

    __ubuf__ T* dstAddr = reinterpret_cast<__ubuf__ T*>(dstTensor.GetPhyAddr());
    __ubuf__ T* srcAddr = reinterpret_cast<__ubuf__ T*>(srcTensor.GetPhyAddr());

    constexpr uint32_t oneRepeatSize = AscendC::GetVecLen() / sizeof(T);
    uint32_t repeatTimes = AscendC::CeilDivision(calCount, oneRepeatSize);

    asc_vf_call<BesselI0Vf<T>>(dstAddr, srcAddr, calCount, oneRepeatSize, repeatTimes);
}

} // namespace AscendC

#endif // IMPL_MATH_BESSEL_I0_BESSEL_I0_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_BESSEL_I0_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_BESSEL_I0_C310_IMPL_H__
#endif
