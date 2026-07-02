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

__simd_vf__ inline void SoftmaxFlashV2NZWithTailUpdate5102VFImpl(
    __ubuf__ half* dstUb, __ubuf__ half* expSumUb, __ubuf__ half* inExpSumUb, __ubuf__ half* maxUb, __ubuf__ half* inMaxUb,
    __ubuf__ half* srcUb, __ubuf__ half* expMaxUb, __ubuf__ half* workUb, const LastAxisShapeND originalSrcShape, const SoftMaxTiling tiling)
{
    uint16_t srcM = tiling.srcM;
    uint16_t originK = originalSrcShape.k;
    uint16_t originM = originalSrcShape.m;
    uint16_t dataBlock = srcM * SOFTMAX_SHAPE_NZ_BASIC_COUNT;
    uint16_t mRepeatTimes = CeilDivision(dataBlock, HALF_REPEAT_SIZE);
    uint16_t kRepeatTimes = originK / SOFTMAX_SHAPE_NZ_BASIC_COUNT;
    uint16_t dtypeBlkStride = HALF_REPEAT_SIZE / DEFAULT_BLK_NUM;

    HalfUnion notNum;
    notNum.i = 0xfc00;  // -inf

    Reg::MaskReg pregDst;
    Reg::MaskReg pregkTail = Reg::MoveMask<uint16_t>();
    Reg::MaskReg pregCnt;
    Reg::MaskReg pregFull = Reg::CreateMask<uint16_t, Reg::MaskPattern::ALL>();
    Reg::MaskReg pregOneBlk = Reg::CreateMask<uint16_t, Reg::MaskPattern::VL16>();
    Reg::RegTensor<half> srcVreg;
    Reg::RegTensor<half> maxVreg;
    Reg::RegTensor<half> inMaxVreg;
    Reg::RegTensor<half> sumVreg;
    Reg::RegTensor<half> tmpVreg;
    Reg::RegTensor<half> minVreg;
    Reg::RegTensor<half> dstVreg;

    // reducemax
    Reg::Duplicate(minVreg, notNum.f);
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        Reg::Duplicate(maxVreg, notNum.f);
        for (uint16_t j = 0; j < kRepeatTimes; ++j) {
            Reg::LoadAlign(srcVreg, srcUb + i * HALF_REPEAT_SIZE + j * dataBlock);
            Reg::Max(maxVreg, maxVreg, srcVreg, pregFull);
        }
        Reg::LoadAlign(srcVreg, srcUb + i * HALF_REPEAT_SIZE + kRepeatTimes * dataBlock);
        Reg::Select(srcVreg, srcVreg, minVreg, pregkTail);
        Reg::Max(maxVreg, maxVreg, srcVreg, pregFull);

        Reg::ReduceMaxWithDataBlock(maxVreg, maxVreg, pregFull);
        Reg::StoreAlign(workUb + i * DEFAULT_BLK_NUM, maxVreg, pregOneBlk);
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    uint32_t sreg = originM * dtypeBlkStride;
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        pregCnt = Reg::UpdateMask<uint16_t>(sreg);
        Reg::LoadAlign<half, Reg::LoadDist::DIST_E2B_B16>(maxVreg, workUb + i * DEFAULT_BLK_NUM);
        Reg::LoadAlign(inMaxVreg, inMaxUb + i * HALF_REPEAT_SIZE);
        Reg::Max(maxVreg, inMaxVreg, maxVreg, pregCnt);
        Reg::StoreAlign(maxUb + i * HALF_REPEAT_SIZE, maxVreg, pregCnt);
        Reg::FusedExpSub(dstVreg, inMaxVreg, maxVreg, pregCnt);
        Reg::StoreAlign(expMaxUb + i * HALF_REPEAT_SIZE, dstVreg, pregCnt);
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    for (uint16_t j = 0; j < kRepeatTimes; ++j) {
        sreg = originM * SOFTMAX_SHAPE_NZ_BASIC_COUNT;
        for (uint16_t i = 0; i < mRepeatTimes; ++i) {
            pregCnt = Reg::UpdateMask<uint16_t>(sreg);
            Reg::LoadAlign(maxVreg, maxUb + i * HALF_REPEAT_SIZE);
            Reg::LoadAlign(srcVreg, srcUb + i * HALF_REPEAT_SIZE + j * dataBlock);
            Reg::FusedExpSub(tmpVreg, srcVreg, maxVreg, pregCnt);
            Reg::StoreAlign(dstUb + i * HALF_REPEAT_SIZE + j * dataBlock, tmpVreg, pregCnt);
        }
    }
    sreg = originM * SOFTMAX_SHAPE_NZ_BASIC_COUNT;
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        pregCnt = Reg::UpdateMask<uint16_t>(sreg);
        Reg::LoadAlign(maxVreg, maxUb + i * HALF_REPEAT_SIZE);
        Reg::LoadAlign(srcVreg, srcUb + i * HALF_REPEAT_SIZE + kRepeatTimes * dataBlock);
        Reg::MaskAnd(pregDst, pregkTail, pregCnt, pregFull);
        Reg::FusedExpSub(tmpVreg, srcVreg, maxVreg, pregDst);
        Reg::StoreAlign(dstUb + i * HALF_REPEAT_SIZE + kRepeatTimes * dataBlock, tmpVreg, pregDst);
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    // reducesum
    Reg::Duplicate(minVreg, 0);
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        Reg::Duplicate(sumVreg, 0);
        for (uint16_t j = 0; j < kRepeatTimes; ++j) {
            Reg::LoadAlign(tmpVreg, dstUb + i * HALF_REPEAT_SIZE + j * dataBlock);
            Reg::Add(sumVreg, sumVreg, tmpVreg, pregFull);
        }
        Reg::LoadAlign(tmpVreg, dstUb + i * HALF_REPEAT_SIZE + kRepeatTimes * dataBlock);
        Reg::Select(tmpVreg, tmpVreg, minVreg, pregkTail);
        Reg::Add(sumVreg, sumVreg, tmpVreg, pregFull);

        Reg::ReduceSumWithDataBlock(sumVreg, sumVreg, pregFull);
        Reg::StoreAlign(workUb + i * DEFAULT_BLK_NUM, sumVreg, pregOneBlk);
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    sreg = originM * dtypeBlkStride;
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        pregCnt = Reg::UpdateMask<uint16_t>(sreg);
        Reg::LoadAlign<half, Reg::LoadDist::DIST_E2B_B16>(sumVreg, workUb + i * DEFAULT_BLK_NUM);
        Reg::LoadAlign(tmpVreg, inExpSumUb + i * HALF_REPEAT_SIZE);
        Reg::LoadAlign(maxVreg, expMaxUb + i * HALF_REPEAT_SIZE);
        Reg::Mul(dstVreg, maxVreg, tmpVreg, pregCnt);
        Reg::Add(sumVreg, sumVreg, dstVreg, pregCnt);
        Reg::StoreAlign(expSumUb + i * HALF_REPEAT_SIZE, sumVreg, pregCnt);
    }
}

__aicore__ inline void SoftmaxFlashV2NZWithTailUpdate5102Impl(
    const LocalTensor<half>& dstTensor, const LocalTensor<half>& expSumTensor,
    const LocalTensor<half>& maxTensor, const LocalTensor<half>& srcTensor, const LocalTensor<half>& expMaxTensor,
    const LocalTensor<half>& inExpSumTensor, const LocalTensor<half>& inMaxTensor,
    const LocalTensor<float>& workLocal, const LastAxisShapeND& originalSrcShape, const SoftMaxTiling& tiling)
{
    uint16_t originK = originalSrcShape.k;

    uint64_t mask[2] = {0, 0};
    CreateSpecialFormatMask(
        mask[0], originK % SOFTMAX_SHAPE_NZ_BASIC_COUNT, FLOAT_REPEAT_SIZE / SOFTMAX_SHAPE_NZ_BASIC_COUNT);
    CreateSpecialFormatMask(
        mask[1], originK % SOFTMAX_SHAPE_NZ_BASIC_COUNT, FLOAT_REPEAT_SIZE / SOFTMAX_SHAPE_NZ_BASIC_COUNT);
    SetVectorMask<uint16_t>(mask[1], mask[0]);

    __ubuf__ half* dstUb = (__ubuf__ half*)dstTensor.GetPhyAddr();
    __ubuf__ half* expSumUb = (__ubuf__ half*)expSumTensor.GetPhyAddr();
    __ubuf__ half* inExpSumUb = (__ubuf__ half*)inExpSumTensor.GetPhyAddr();
    __ubuf__ half* maxUb = (__ubuf__ half*)maxTensor.GetPhyAddr();
    __ubuf__ half* inMaxUb = (__ubuf__ half*)inMaxTensor.GetPhyAddr();
    __ubuf__ half* srcUb = (__ubuf__ half*)srcTensor.GetPhyAddr();
    __ubuf__ half* expMaxUb = (__ubuf__ half*)expMaxTensor.GetPhyAddr();
    __ubuf__ half* workUb = (__ubuf__ half*)workLocal.GetPhyAddr();
    SoftmaxFlashV2NZWithTailUpdate5102VFImpl(
        dstUb, expSumUb, inExpSumUb, maxUb, inMaxUb, srcUb, expMaxUb, workUb, originalSrcShape, tiling);
}

__simd_vf__ inline void SoftmaxFlashV2NZUpdate5102VFImpl(
    __ubuf__ half* dstUb, __ubuf__ half* expSumUb, __ubuf__ half* inExpSumUb, __ubuf__ half* maxUb, __ubuf__ half* inMaxUb,
    __ubuf__ half* srcUb, __ubuf__ half* expMaxUb, __ubuf__ half* workUb, const LastAxisShapeND originalSrcShape, const SoftMaxTiling tiling)
{
    uint16_t srcM = tiling.srcM;
    uint16_t originK = originalSrcShape.k;
    uint16_t originM = originalSrcShape.m;

    uint16_t dataBlock = srcM * SOFTMAX_SHAPE_NZ_BASIC_COUNT;
    uint16_t mRepeatTimes = CeilDivision(dataBlock, HALF_REPEAT_SIZE);
    uint16_t kRepeatTimes = originK / SOFTMAX_SHAPE_NZ_BASIC_COUNT;
    uint16_t dtypeBlkStride = HALF_REPEAT_SIZE / DEFAULT_BLK_NUM;

    HalfUnion notNum;
    notNum.i = 0xfc00;  // -inf

    Reg::MaskReg pregCnt;
    Reg::MaskReg pregFull = Reg::CreateMask<uint16_t, Reg::MaskPattern::ALL>();
    Reg::MaskReg pregOneBlk = Reg::CreateMask<uint16_t, Reg::MaskPattern::VL16>();
    Reg::RegTensor<half> srcVreg;
    Reg::RegTensor<half> maxVreg;
    Reg::RegTensor<half> inMaxVreg;
    Reg::RegTensor<half> sumVreg;
    Reg::RegTensor<half> tmpVreg;
    Reg::RegTensor<half> dstVreg;

    // reducemax
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        Reg::Duplicate(maxVreg, notNum.f);
        for (uint16_t j = 0; j < kRepeatTimes; ++j) {
            Reg::LoadAlign(srcVreg, srcUb + i * HALF_REPEAT_SIZE + j * dataBlock);
            Reg::Max(maxVreg, maxVreg, srcVreg, pregFull);
        }
        Reg::ReduceMaxWithDataBlock(maxVreg, maxVreg, pregFull);
        Reg::StoreAlign(workUb + i * DEFAULT_BLK_NUM, maxVreg, pregOneBlk);
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    uint32_t sreg = originM * dtypeBlkStride;
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        pregCnt = Reg::UpdateMask<uint16_t>(sreg);
        Reg::LoadAlign<half, Reg::LoadDist::DIST_E2B_B16>(maxVreg, workUb + i * DEFAULT_BLK_NUM);
        Reg::LoadAlign(inMaxVreg, inMaxUb + i * HALF_REPEAT_SIZE);
        Reg::Max(maxVreg, inMaxVreg, maxVreg, pregCnt);
        Reg::StoreAlign(maxUb + i * HALF_REPEAT_SIZE, maxVreg, pregCnt);
        Reg::FusedExpSub(dstVreg, inMaxVreg, maxVreg, pregCnt);
        Reg::StoreAlign(expMaxUb + i * HALF_REPEAT_SIZE, dstVreg, pregCnt);
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    for (uint16_t j = 0; j < kRepeatTimes; ++j) {
        sreg = originM * SOFTMAX_SHAPE_NZ_BASIC_COUNT;
        for (uint16_t i = 0; i < mRepeatTimes; ++i) {
            pregCnt = Reg::UpdateMask<uint16_t>(sreg);
            Reg::LoadAlign(maxVreg, maxUb + i * HALF_REPEAT_SIZE);
            Reg::LoadAlign(srcVreg, srcUb + i * HALF_REPEAT_SIZE + j * dataBlock);
            Reg::FusedExpSub(tmpVreg, srcVreg, maxVreg, pregCnt);
            Reg::StoreAlign(dstUb + i * HALF_REPEAT_SIZE + j * dataBlock, tmpVreg, pregCnt);
        }
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    // reducesum
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        Reg::Duplicate(sumVreg, 0);
        for (uint16_t j = 0; j < kRepeatTimes; ++j) {
            Reg::LoadAlign(tmpVreg, dstUb + i * HALF_REPEAT_SIZE + j * dataBlock);
            Reg::Add(sumVreg, sumVreg, tmpVreg, pregFull);
        }
        Reg::ReduceSumWithDataBlock(sumVreg, sumVreg, pregFull);
        Reg::StoreAlign(workUb + i * DEFAULT_BLK_NUM, sumVreg, pregOneBlk);
    }

    Reg::LocalMemBar<Reg::MemType::VEC_STORE, Reg::MemType::VEC_LOAD>();

    sreg = originM * dtypeBlkStride;
    for (uint16_t i = 0; i < mRepeatTimes; ++i) {
        pregCnt = Reg::UpdateMask<uint16_t>(sreg);
        Reg::LoadAlign<half, Reg::LoadDist::DIST_E2B_B16>(sumVreg, workUb + i * DEFAULT_BLK_NUM);
        Reg::LoadAlign(tmpVreg, inExpSumUb + i * HALF_REPEAT_SIZE);
        Reg::LoadAlign(maxVreg, expMaxUb + i * HALF_REPEAT_SIZE);
        Reg::Mul(dstVreg, maxVreg, tmpVreg, pregCnt);
        Reg::Add(sumVreg, sumVreg, dstVreg, pregCnt);
        Reg::StoreAlign(expSumUb + i * HALF_REPEAT_SIZE, sumVreg, pregCnt);
    }
}

__aicore__ inline void SoftmaxFlashV2NZUpdate5102Impl(
    const LocalTensor<half>& dstTensor, const LocalTensor<half>& expSumTensor,
    const LocalTensor<half>& maxTensor, const LocalTensor<half>& srcTensor, const LocalTensor<half>& expMaxTensor,
    const LocalTensor<half>& inExpSumTensor, const LocalTensor<half>& inMaxTensor,
    const LocalTensor<float>& workLocal, const LastAxisShapeND& originalSrcShape, const SoftMaxTiling& tiling)
{
    __ubuf__ half* dstUb = (__ubuf__ half*)dstTensor.GetPhyAddr();
    __ubuf__ half* expSumUb = (__ubuf__ half*)expSumTensor.GetPhyAddr();
    __ubuf__ half* inExpSumUb = (__ubuf__ half*)inExpSumTensor.GetPhyAddr();
    __ubuf__ half* maxUb = (__ubuf__ half*)maxTensor.GetPhyAddr();
    __ubuf__ half* inMaxUb = (__ubuf__ half*)inMaxTensor.GetPhyAddr();
    __ubuf__ half* srcUb = (__ubuf__ half*)srcTensor.GetPhyAddr();
    __ubuf__ half* expMaxUb = (__ubuf__ half*)expMaxTensor.GetPhyAddr();
    __ubuf__ half* workUb = (__ubuf__ half*)workLocal.GetPhyAddr();

    SoftmaxFlashV2NZUpdate5102VFImpl(
        dstUb, expSumUb, inExpSumUb, maxUb, inMaxUb, srcUb, expMaxUb, workUb, originalSrcShape, tiling);
}

__simd_vf__ inline void SoftmaxFlashV2NDUpdate5102VFImpl(
    __ubuf__ half* dstUb, __ubuf__ half* expSumUb, __ubuf__ half* inExpSumUb, __ubuf__ half* maxUb, __ubuf__ half* inMaxUb,
    __ubuf__ half* srcUb, __ubuf__ half* expMaxUb, const LastAxisShapeND originalSrcShape, const SoftMaxTiling tiling)
{
    uint16_t srcM = originalSrcShape.m;
    uint16_t srcK = tiling.srcK;
    uint16_t originK = originalSrcShape.k;
    uint16_t repeatTimesF16 = CeilDivision(originK, HALF_REPEAT_SIZE);
    uint16_t blockStride = B16_DATA_NUM_PER_BLOCK;
    HalfUnion notNum;
    notNum.i = 0xfc00;  // -inf

    Reg::MaskReg pregFull = Reg::CreateMask<uint16_t, Reg::MaskPattern::ALL>();
    Reg::MaskReg pregOneBlk = Reg::CreateMask<uint16_t, Reg::MaskPattern::VL16>();
    Reg::RegTensor<half> srcVreg;
    Reg::RegTensor<half> maxVreg;
    Reg::RegTensor<half> expMaxVreg;
    Reg::RegTensor<half> tmpVreg;
    Reg::RegTensor<half> sumVreg;
    Reg::RegTensor<half> dstVreg;

    for (uint16_t i = 0; i < srcM; ++i) {
        Reg::Duplicate(maxVreg, notNum.f);
        for (uint16_t j = 0; j < repeatTimesF16; ++j) {
            Reg::LoadAlign(srcVreg, srcUb + i * srcK + j * HALF_REPEAT_SIZE);
            Reg::Max(maxVreg, maxVreg, srcVreg, pregFull);
        }
        Reg::ReduceMax(maxVreg, maxVreg, pregFull);
        Reg::Duplicate(maxVreg, maxVreg, pregOneBlk);
        Reg::LoadAlign(tmpVreg, inMaxUb + i * blockStride);
        Reg::Max(maxVreg, maxVreg, tmpVreg, pregOneBlk);
        Reg::StoreAlign(maxUb + i * blockStride, maxVreg, pregOneBlk);

        Reg::FusedExpSub(expMaxVreg, tmpVreg, maxVreg, pregOneBlk);
        Reg::StoreAlign(expMaxUb + i * blockStride, expMaxVreg, pregOneBlk);

        Reg::Duplicate(sumVreg, 0);
        Reg::Duplicate(maxVreg, maxVreg, pregFull);
        for (uint16_t j = 0; j < repeatTimesF16; ++j) {
            Reg::LoadAlign(srcVreg, srcUb + i * srcK + j * HALF_REPEAT_SIZE);
            Reg::FusedExpSub(tmpVreg, srcVreg, maxVreg, pregFull);
            Reg::StoreAlign(dstUb + i * srcK + j * HALF_REPEAT_SIZE, tmpVreg, pregFull);
            Reg::Add(sumVreg, sumVreg, tmpVreg, pregFull);
        }
        Reg::ReduceSum(sumVreg, sumVreg, pregFull);
        Reg::Duplicate(sumVreg, sumVreg, pregOneBlk);
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

    SoftmaxFlashV2NDUpdate5102VFImpl(
        dstUb, expSumUb, inExpSumUb, maxUb, inMaxUb, srcUb, expMaxUb, originalSrcShape, tiling);
}

__simd_vf__ inline void SoftmaxFlashV2NDWithTailUpdate5102VFImpl(
    __ubuf__ half* dstUb, __ubuf__ half* expSumUb, __ubuf__ half* inExpSumUb, __ubuf__ half* maxUb, __ubuf__ half* inMaxUb,
    __ubuf__ half* srcUb, __ubuf__ half* expMaxUb, const LastAxisShapeND originalSrcShape, const SoftMaxTiling tiling)
{
    uint16_t srcM = originalSrcShape.m;
    uint16_t srcK = tiling.srcK;
    uint16_t originK = originalSrcShape.k;
    uint16_t repeatTimes = CeilDivision(originK, HALF_REPEAT_SIZE);
    uint16_t blockStride = B16_DATA_NUM_PER_BLOCK;
    HalfUnion notNum;
    notNum.i = 0xfc00;  // -inf

    Reg::MaskReg pregCnt;
    Reg::MaskReg pregFull = Reg::CreateMask<uint16_t, Reg::MaskPattern::ALL>();
    Reg::MaskReg pregOneBlk = Reg::CreateMask<uint16_t, Reg::MaskPattern::VL16>();
    Reg::RegTensor<half> srcVreg;
    Reg::RegTensor<half> maxVreg;
    Reg::RegTensor<half> expMaxVreg;
    Reg::RegTensor<half> sumVreg;
    Reg::RegTensor<half> tmpVreg;
    Reg::RegTensor<half> minVreg;
    Reg::RegTensor<half> dstVreg;

    Reg::Duplicate(minVreg, notNum.f);
    for (uint16_t i = 0; i < srcM; ++i) {
        uint32_t sreg = originK;
        Reg::Duplicate(maxVreg, notNum.f);
        for (uint16_t j = 0; j < static_cast<uint16_t>(repeatTimes - 1); ++j) {
            pregCnt = Reg::UpdateMask<uint16_t>(sreg);
            Reg::LoadAlign(srcVreg, srcUb + i * srcK + j * HALF_REPEAT_SIZE);
            Reg::Max(maxVreg, maxVreg, srcVreg, pregCnt);
        }
        pregCnt = Reg::UpdateMask<uint16_t>(sreg);
        Reg::LoadAlign(srcVreg, srcUb + i * srcK + (repeatTimes - 1) * HALF_REPEAT_SIZE);
        Reg::Select(srcVreg, srcVreg, minVreg, pregCnt);
        Reg::Max(maxVreg, maxVreg, srcVreg, pregFull);

        Reg::ReduceMax(maxVreg, maxVreg, pregFull);
        Reg::Duplicate(maxVreg, maxVreg, pregOneBlk);
        Reg::LoadAlign(tmpVreg, inMaxUb + i * blockStride);
        Reg::Max(maxVreg, maxVreg, tmpVreg, pregOneBlk);
        Reg::StoreAlign(maxUb + i * blockStride, maxVreg, pregOneBlk);

        Reg::FusedExpSub(expMaxVreg, tmpVreg, maxVreg, pregOneBlk);
        Reg::StoreAlign(expMaxUb + i * blockStride, expMaxVreg, pregOneBlk);

        Reg::Duplicate(sumVreg, 0);
        Reg::Duplicate(maxVreg, maxVreg, pregFull);
        sreg = originK;
        for (uint16_t j = 0; j < repeatTimes; ++j) {
            pregCnt = Reg::UpdateMask<uint16_t>(sreg);
            Reg::LoadAlign(srcVreg, srcUb + i * srcK + j * HALF_REPEAT_SIZE);
            Reg::FusedExpSub(tmpVreg, srcVreg, maxVreg, pregCnt);
            Reg::StoreAlign(dstUb + i * srcK + j * HALF_REPEAT_SIZE, tmpVreg, pregCnt);
            Reg::Add(sumVreg, sumVreg, tmpVreg, pregFull);
        }
        Reg::ReduceSum(sumVreg, sumVreg, pregFull);
        Reg::Duplicate(sumVreg, sumVreg, pregOneBlk);
        Reg::LoadAlign(tmpVreg, inExpSumUb + i * blockStride);
        Reg::Mul(tmpVreg, expMaxVreg, tmpVreg, pregOneBlk);
        Reg::Add(sumVreg, sumVreg, tmpVreg, pregOneBlk);
        Reg::StoreAlign(expSumUb + i * blockStride, sumVreg, pregOneBlk);
    }
}

__aicore__ inline void SoftmaxFlashV2NDWithTailUpdate5102Impl(
    const LocalTensor<half>& dstTensor, const LocalTensor<half>& expSumTensor,
    const LocalTensor<half>& maxTensor, const LocalTensor<half>& srcTensor, const LocalTensor<half>& expMaxTensor,
    const LocalTensor<half>& inExpSumTensor, const LocalTensor<half>& inMaxTensor,
    const LocalTensor<float>& workLocal, const LastAxisShapeND& originalSrcShape, const SoftMaxTiling& tiling)
{
    __ubuf__ half* dstUb = (__ubuf__ half*)dstTensor.GetPhyAddr();
    __ubuf__ half* expSumUb = (__ubuf__ half*)expSumTensor.GetPhyAddr();
    __ubuf__ half* inExpSumUb = (__ubuf__ half*)inExpSumTensor.GetPhyAddr();
    __ubuf__ half* maxUb = (__ubuf__ half*)maxTensor.GetPhyAddr();
    __ubuf__ half* inMaxUb = (__ubuf__ half*)inMaxTensor.GetPhyAddr();
    __ubuf__ half* srcUb = (__ubuf__ half*)srcTensor.GetPhyAddr();
    __ubuf__ half* expMaxUb = (__ubuf__ half*)expMaxTensor.GetPhyAddr();

    SoftmaxFlashV2NDWithTailUpdate5102VFImpl(
        dstUb, expSumUb, inExpSumUb, maxUb, inMaxUb, srcUb, expMaxUb, originalSrcShape, tiling);
}

} // namespace AscendC

#endif // IMPL_ACTIVATION_SOFTMAX_5102_SOFTMAX_FLASHV2_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SOFTMAX_FLASHV2_5102_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SOFTMAX_FLASHV2_5102_IMPL_H__
#endif
