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
 * \file ascend_antiquant_innerloop_impl.h
 * \brief
 */
#ifndef IMPL_ADV_API_DETAIL_QUANTIZATION_ANTIQUANT_ASCEND_ANTIQUANT_INNERLOOP_IMPL_H
#define IMPL_ADV_API_DETAIL_QUANTIZATION_ANTIQUANT_ASCEND_ANTIQUANT_INNERLOOP_IMPL_H

namespace AscendC {
__aicore__ inline void AntiQuantInnerLoopF16(const LocalTensor<half> &dst, const LocalTensor<half> &src,
    const LocalTensor<half> &offset, const LocalTensor<half> &scale, const LocalTensor<uint8_t> &sharedTmpBuffer,
    const BinaryRepeatParams &binaryParams, const uint32_t calCount)
{
    SetVectorMask<half, MaskMode::COUNTER>(0, calCount);
    Add<half, false>(dst, offset, src, MASK_PLACEHOLDER, 1, binaryParams);
    PipeBarrier<PIPE_V>();
    Mul<half, false>(dst, scale, dst, MASK_PLACEHOLDER, 1, binaryParams);
    PipeBarrier<PIPE_V>();
}

template <typename SrcType, bool withOffset = true>
__aicore__ inline void AntiQuantInnerLoop(const LocalTensor<half> &dst, const LocalTensor<SrcType> &src,
    const LocalTensor<half> &offset, const LocalTensor<half> &scale, const LocalTensor<uint8_t> &sharedTmpBuffer,
    const UnaryRepeatParams &unaryParamsCastSrc, const BinaryRepeatParams &binaryParams, const uint32_t calCount)
{
    SetVectorMask<half, MaskMode::COUNTER>(0, calCount);
    Cast<half, SrcType, false>(dst, src, RoundMode::CAST_NONE, MASK_PLACEHOLDER, 1, unaryParamsCastSrc);
    PipeBarrier<PIPE_V>();
    if constexpr (withOffset) {
        Add<half, false>(dst, offset, dst, MASK_PLACEHOLDER, 1, binaryParams);
        PipeBarrier<PIPE_V>();
    }
    Mul<half, false>(dst, scale, dst, MASK_PLACEHOLDER, 1, binaryParams);
    PipeBarrier<PIPE_V>();
}

template <typename SrcType, bool withOffset = true>
__aicore__ inline void AntiQuantInnerLoop(const LocalTensor<half> &dst, const LocalTensor<SrcType> &src,
    const half offset, const half scale, const LocalTensor<uint8_t> &sharedTmpBuffer,
    const UnaryRepeatParams &unaryParamsCastSrc, const UnaryRepeatParams &unaryParamsScalar, const uint32_t calCount)
{
    SetVectorMask<half, MaskMode::COUNTER>(0, calCount);
    Cast<half, SrcType, false>(dst, src, RoundMode::CAST_NONE, MASK_PLACEHOLDER, 1, unaryParamsCastSrc);
    PipeBarrier<PIPE_V>();
    if constexpr (withOffset) {
        Adds<half, false>(dst, dst, offset, MASK_PLACEHOLDER, 1, unaryParamsScalar);
        PipeBarrier<PIPE_V>();
    }
    Muls<half, false>(dst, dst, scale, MASK_PLACEHOLDER, 1, unaryParamsScalar);
    PipeBarrier<PIPE_V>();
}

template <typename SrcType, typename DstType, bool withOffset = true>
__aicore__ inline void AntiQuantOuterLoop(const LocalTensor<DstType> &dst, const LocalTensor<SrcType> &src,
    const LocalTensor<DstType> &offset, const LocalTensor<DstType> &scale, const LocalTensor<uint8_t> &sharedTmpBuffer,
    const uint32_t calCount)
{
    UnaryRepeatParams unaryParamsCastSrc;          // srcType -> FP16
    if constexpr(IsSameType<SrcType, int8_t>::value) {
        unaryParamsCastSrc.srcRepStride = HALF_DEFAULT_REPEAT_STRIDE;
    } else {
        unaryParamsCastSrc.srcRepStride = ONE_FOURTH_DEFAULT_REPEAT_STRIDE;
    }
    BinaryRepeatParams binaryParams;              // used for add and mul
    if constexpr (IsSameType<DstType, half>::value) {
        AntiQuantInnerLoop<SrcType, withOffset>(dst, src, offset, scale, sharedTmpBuffer, unaryParamsCastSrc,
            binaryParams, calCount);
    } else {    // dstType = bfloat16_t
        uint32_t tmpSize = sharedTmpBuffer.GetSize() / sizeof(DstType) / ANTIQUANT_FOUR;
        uint32_t loopCount = calCount / tmpSize;
        uint32_t tailSize = calCount % tmpSize;

        UnaryRepeatParams unaryParamsFP32ToDst;   // FP32 -> BF16
        unaryParamsFP32ToDst.dstRepStride = HALF_DEFAULT_REPEAT_STRIDE;
        UnaryRepeatParams unaryParamsToFP32;      // FP16 -> FP32
        unaryParamsToFP32.srcRepStride = HALF_DEFAULT_REPEAT_STRIDE;

        for (uint32_t i = 0; i < loopCount; i++) {
            AntiQuantInnerLoop<SrcType, withOffset>(dst[i * tmpSize], src[i * tmpSize], offset, scale, sharedTmpBuffer,
                unaryParamsCastSrc, unaryParamsToFP32, unaryParamsFP32ToDst, binaryParams, tmpSize);
        }
        if (tailSize > 0) {
            AntiQuantInnerLoop<SrcType, withOffset>(dst[loopCount * tmpSize], src[loopCount * tmpSize], offset, scale,
                sharedTmpBuffer, unaryParamsCastSrc, unaryParamsToFP32, unaryParamsFP32ToDst, binaryParams, tailSize);
        }
    }
}

template <typename SrcType, typename DstType, bool withOffset = true>
__aicore__ inline void AntiQuantOuterLoop(const LocalTensor<DstType> &dst, const LocalTensor<SrcType> &src,
    const DstType offset, const DstType scale, const LocalTensor<uint8_t> &sharedTmpBuffer, const uint32_t calCount)
{
    UnaryRepeatParams unaryParamsCastSrc;
    if constexpr(IsSameType<SrcType, int8_t>::value) {
        unaryParamsCastSrc.srcRepStride = HALF_DEFAULT_REPEAT_STRIDE;
    } else {
        unaryParamsCastSrc.srcRepStride = ONE_FOURTH_DEFAULT_REPEAT_STRIDE;
    }
    UnaryRepeatParams unaryParamsScalar;
    if constexpr (IsSameType<DstType, half>::value) {
        AntiQuantInnerLoop<SrcType, withOffset>(dst, src, offset, scale, sharedTmpBuffer, unaryParamsCastSrc,
            unaryParamsScalar, calCount);
    } else {
        uint32_t tmpSize = sharedTmpBuffer.GetSize() / sizeof(DstType) / ANTIQUANT_FOUR;
        uint32_t loopCount = calCount / tmpSize;
        uint32_t tailSize = calCount % tmpSize;

        UnaryRepeatParams unaryParamsToFP32;
        unaryParamsToFP32.srcRepStride = HALF_DEFAULT_REPEAT_STRIDE;
        UnaryRepeatParams unaryParamsFP32ToDst;
        unaryParamsFP32ToDst.dstRepStride = HALF_DEFAULT_REPEAT_STRIDE;

        for (uint32_t i = 0; i < loopCount; i++) {
            AntiQuantInnerLoop<SrcType, withOffset>(dst[i * tmpSize], src[i * tmpSize], offset, scale, sharedTmpBuffer,
                unaryParamsCastSrc, unaryParamsToFP32, unaryParamsFP32ToDst, unaryParamsScalar, tmpSize);
        }
        if (tailSize > 0) {
            AntiQuantInnerLoop<SrcType, withOffset>(dst[loopCount * tmpSize], src[loopCount * tmpSize], offset, scale,
                sharedTmpBuffer, unaryParamsCastSrc, unaryParamsToFP32, unaryParamsFP32ToDst, unaryParamsScalar,
                tailSize);
        }
    }
}
} // namespace AscendC
#endif // IMPL_ADV_API_DETAIL_QUANTIZATION_ANTIQUANT_ASCEND_ANTIQUANT_INNERLOOP_IMPL_H

