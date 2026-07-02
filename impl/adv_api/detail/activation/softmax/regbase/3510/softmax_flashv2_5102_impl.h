/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file softmax_flashv2_5102_impl.h
 * \brief SoftmaxFlashV2 5102-specific implementation hooks.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/activation/softmax/regbase/3510/softmax_flashv2_5102_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/activation/softmaxflashv2.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SOFTMAX_FLASHV2_5102_IMPL_H__
#endif

#ifndef IMPL_ACTIVATION_SOFTMAX_5102_SOFTMAX_FLASHV2_IMPL_H
#define IMPL_ACTIVATION_SOFTMAX_5102_SOFTMAX_FLASHV2_IMPL_H

namespace AscendC {

__aicore__ inline void SoftmaxFlashV2NZUpdate5102Impl(
    const LocalTensor<half>& dstTensor, const LocalTensor<half>& expSumTensor,
    const LocalTensor<half>& maxTensor, const LocalTensor<half>& srcTensor, const LocalTensor<half>& expMaxTensor,
    const LocalTensor<half>& inExpSumTensor, const LocalTensor<half>& inMaxTensor,
    const LocalTensor<float>& workLocal, const LastAxisShapeND& originalSrcShape, const SoftMaxTiling& tiling)
{
    (void)dstTensor;
    (void)expSumTensor;
    (void)maxTensor;
    (void)srcTensor;
    (void)expMaxTensor;
    (void)inExpSumTensor;
    (void)inMaxTensor;
    (void)workLocal;
    (void)originalSrcShape;
    (void)tiling;
    // TODO: Add 5102 half/half update NZ implementation.
}

__aicore__ inline void SoftmaxFlashV2NZWithTailUpdate5102Impl(
    const LocalTensor<half>& dstTensor, const LocalTensor<half>& expSumTensor,
    const LocalTensor<half>& maxTensor, const LocalTensor<half>& srcTensor, const LocalTensor<half>& expMaxTensor,
    const LocalTensor<half>& inExpSumTensor, const LocalTensor<half>& inMaxTensor,
    const LocalTensor<float>& workLocal, const LastAxisShapeND& originalSrcShape, const SoftMaxTiling& tiling)
{
    (void)dstTensor;
    (void)expSumTensor;
    (void)maxTensor;
    (void)srcTensor;
    (void)expMaxTensor;
    (void)inExpSumTensor;
    (void)inMaxTensor;
    (void)workLocal;
    (void)originalSrcShape;
    (void)tiling;
    // TODO: Add 5102 half/half update NZ tail implementation.
}

__simd_vf__ inline void SoftmaxFlashV2NDUpdateVFImpl(
    __ubuf__ half* dstUb, __ubuf__ half* expSumUb, __ubuf__ half* inExpSumUb, __ubuf__ half* maxUb, __ubuf__ half* inMaxUb,
    __ubuf__ half* srcUb, __ubuf__ half* expMaxUb, const LastAxisShapeND originalSrcShape, const SoftMaxTiling tiling)
{
    uint16_t srcM = originalSrcShape.m;
    uint16_t srcK = tiling.srcK;
    uint16_t originK = originalSrcShape.k;
    uint16_t repeatTimesF32 = CeilDivision(originK, FLOAT_REPEAT_SIZE);
    uint16_t repeatTimesF16 = CeilDivision(originK, HALF_REPEAT_SIZE);
    uint16_t blockStride = B16_DATA_NUM_PER_BLOCK;
    HalfUnion notNum;
    notNum.i = 0xfc00;  // -inf

    Reg::MaskReg pregFull = Reg::CreateMask<uint16_t, Reg::MaskPattern::ALL>();
    // Reg::MaskReg pregFullF32 = Reg::CreateMask<uint32_t, Reg::MaskPattern::ALL>();
    Reg::MaskReg pregOneBlk, pregOneBlkF32;
    pregOneBlk = Reg::CreateMask<uint16_t, Reg::MaskPattern::VL16>();
    // pregOneBlkF32 = Reg::CreateMask<uint32_t, Reg::MaskPattern::VL16>();
    Reg::RegTensor<half> srcVreg;
    Reg::RegTensor<half> maxVreg;
    Reg::RegTensor<half> expMaxVreg;
    Reg::RegTensor<half> tmpVreg;
    Reg::RegTensor<half> sumVreg;
    Reg::RegTensor<half> dstVreg;

    for (uint16_t i = 0; i < srcM; ++i) {
        Duplicate(maxVreg, notNum.f);
        for (uint16_t j = 0; j < repeatTimesF16; ++j) {
            Reg::LoadAlign(srcVreg, srcUb + i * srcK + j * HALF_REPEAT_SIZE);
            Reg::Max(maxVreg, maxVreg, srcVreg, pregFull);
        }
        Reg::ReduceMax(maxVreg, maxVreg, pregFull);
        Duplicate(maxVreg, maxVreg, pregOneBlk);
        Reg::LoadAlign(tmpVreg, inMaxUb + i * blockStride);
        Reg::Max(maxVreg, maxVreg, tmpVreg, pregOneBlk);
        Reg::StoreAlign(maxUb + i * blockStride, maxVreg, pregOneBlk);

        Reg::FusedExpSub(expMaxVreg, tmpVreg, maxVreg, pregOneBlk);
        Reg::StoreAlign(expMaxUb + i * blockStride, expMaxVreg, pregOneBlk);

        Duplicate(sumVreg, 0);
        Duplicate(maxVreg, maxVreg, pregFull);
        for (uint16_t j = 0; j < repeatTimesF16; ++j) {
            Reg::LoadAlign(srcVreg, srcUb + i * srcK + j * HALF_REPEAT_SIZE);
            Reg::FusedExpSub(tmpVreg, srcVreg, maxVreg, pregFull);
            Reg::StoreAlign(dstUb + i * srcK + j * HALF_REPEAT_SIZE, tmpVreg, pregFull);
            Reg::Add(sumVreg, sumVreg, tmpVreg, pregFull);
        }
        Reg::ReduceSum(sumVreg, sumVreg, pregFull);
        Duplicate(sumVreg, sumVreg, pregOneBlk);
        Reg::LoadAlign(tmpVreg, inExpSumUb + i * blockStride);
        Reg::Mul(tmpVreg, expMaxVreg, tmpVreg, pregOneBlk);
        Reg::Add(sumVreg, sumVreg, tmpVreg, pregOneBlk);
        Reg::StoreAlign(expSumUb + i * blockStride, sumVreg, pregOneBlk);
    }
}

__aicore__ inline void SoftmaxFlashV2NDUpdate5102Impl(
    const LocalTensor<half>& dstTensor, const LocalTensor<half>& expSumTensor, const LocalTensor<half>& maxTensor,
    const LocalTensor<half>& srcTensor, const LocalTensor<half>& expMaxTensor, const LocalTensor<half>& inExpSumTensor,
    const LocalTensor<half>& inMaxTensor, const LocalTensor<float>& workLocal, const LastAxisShapeND& originalSrcShape,
    const SoftMaxTiling& tiling)
{
    __ubuf__ half* dstUb = (__ubuf__ half*)dstTensor.GetPhyAddr();
    __ubuf__ half* expSumUb = (__ubuf__ half*)expSumTensor.GetPhyAddr();
    __ubuf__ half* inExpSumUb = (__ubuf__ half*)inExpSumTensor.GetPhyAddr();
    __ubuf__ half* maxUb = (__ubuf__ half*)maxTensor.GetPhyAddr();
    __ubuf__ half* inMaxUb = (__ubuf__ half*)inMaxTensor.GetPhyAddr();
    __ubuf__ half* srcUb = (__ubuf__ half*)srcTensor.GetPhyAddr();
    __ubuf__ half* expMaxUb = (__ubuf__ half*)expMaxTensor.GetPhyAddr();

    SoftmaxFlashV2NDUpdateVFImpl(
        dstUb, expSumUb, inExpSumUb, maxUb, inMaxUb, srcUb, expMaxUb, originalSrcShape, tiling);
}

__aicore__ inline void SoftmaxFlashV2NDWithTailUpdate5102Impl(
    const LocalTensor<half>& dstTensor, const LocalTensor<half>& expSumTensor,
    const LocalTensor<half>& maxTensor, const LocalTensor<half>& srcTensor, const LocalTensor<half>& expMaxTensor,
    const LocalTensor<half>& inExpSumTensor, const LocalTensor<half>& inMaxTensor,
    const LocalTensor<float>& workLocal, const LastAxisShapeND& originalSrcShape, const SoftMaxTiling& tiling)
{
    (void)dstTensor;
    (void)expSumTensor;
    (void)maxTensor;
    (void)srcTensor;
    (void)expMaxTensor;
    (void)inExpSumTensor;
    (void)inMaxTensor;
    (void)workLocal;
    (void)originalSrcShape;
    (void)tiling;
    // TODO: Add 5102 half/half update ND tail implementation.
}

} // namespace AscendC

#endif // IMPL_ACTIVATION_SOFTMAX_5102_SOFTMAX_FLASHV2_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SOFTMAX_FLASHV2_5102_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SOFTMAX_FLASHV2_5102_IMPL_H__
#endif
