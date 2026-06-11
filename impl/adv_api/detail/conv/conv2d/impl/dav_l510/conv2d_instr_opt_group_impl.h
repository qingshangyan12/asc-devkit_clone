/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * \file conv_intr_opt_group_impl.h
 * \brief
 */

#ifndef CONV_INSTR_OPT_GROUP_IMPL_H
#define CONV_INSTR_OPT_GROUP_IMPL_H

#include "../../conv_config.h"
#include "../../conv_util.h"

namespace ConvFunc {
using namespace AscendC;
using namespace conv;

template <class Intf>
class LoadGM2UBTools {
public:
    __aicore__ inline LoadGM2UBTools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void LoadGM2UB(const typename Intf::ContextType &ctx)
    {
        // [zj]ToDo: kernel
        // if (unlikely(ctx.groupOptIter == ctx.vecId)) {
        //     if constexpr (Intf::isQuantScene) {
        //         LocalTensor<int8_t> zeroTensor = ctx.ndUbBuf.template Get<int8_t>();
        //         Duplicate<int8_t>(zeroTensor, 0, ctx.ubBufSize);
        //     } else {
        //         LocalTensor<typename Intf::WeightT> zeroTensor =
        //             ctx.ndUbBuf.template Get<typename Intf::WeightT>();
        //         Duplicate<typename Intf::WeightT>(zeroTensor, 0, ctx.ubBufSize);
        //     }

        //     // For nddma wait vdup
        //     event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::V_MTE2));
        //     SetFlag<HardEvent::V_MTE2>(eventId);
        //     WaitFlag<HardEvent::V_MTE2>(eventId);

        //     uint64_t srcKSize = ctx.ciPerGroup * ctx.convTiling->kernelHxkernelWxkernelD;
        //     // NDDMA Loop0 params
        //     copyParams.loopInfo.loopSize[NDDMA_LOOP0_INDEX] = srcKSize;
        //     copyParams.loopInfo.loopSrcStride[NDDMA_LOOP0_INDEX] = 1;
        //     copyParams.loopInfo.loopDstStride[NDDMA_LOOP0_INDEX] = 1;
        //     // NDDMA Loop1 params
        //     copyParams.loopInfo.loopSize[NDDMA_LOOP1_INDEX] = ctx.coPerGroup;
        //     copyParams.loopInfo.loopSrcStride[NDDMA_LOOP1_INDEX] = srcKSize;
        //     copyParams.loopInfo.loopDstStride[NDDMA_LOOP1_INDEX] = ctx.kUbSize;
        //     // NDDMA Loop2 params
        //     copyParams.loopInfo.loopSrcStride[NDDMA_LOOP2_INDEX] = ctx.coPerGroup * srcKSize;
        //     copyParams.loopInfo.loopDstStride[NDDMA_LOOP2_INDEX] = ctx.coPerGroup * ctx.kUbSize +
        //                                                            srcKSize;
        //     ndTensor = ctx.ndUbBuf.template Get<typename Intf::WeightT>();
        // }

        // copyParams.loopInfo.loopSize[NDDMA_LOOP2_INDEX] = ctx.singleGroups;
        // DataCopy<typename Intf::WeightT, NDDMA_DIMS, kDefaultMultiCopyConfig>(ndTensor, ctx.bgm, copyParams);
    }

private:
    // typename Intf::ContextType *ctx_;
    // MultiCopyParams<typename Intf::WeightT, NDDMA_DIMS> copyParams;
    LocalTensor<typename Intf::WeightT> ndTensor;
};

template <class Intf>
class TransND2NZTools {
public:
    __aicore__ inline TransND2NZTools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;

        indexTensor = ctx.indexUbBuf.template Get<IndexT>();
        ndTensor = ctx.ndUbBuf.template Get<SrcT>();

        if constexpr (Intf::isQuantScene) {
            coOptLoopTimes = ctx.co1Opt * B8_CO0_LOOP_TIMES;
            coPerReg = BLOCK_L0_N / B8_CO0_LOOP_TIMES;
        } else {
            coOptLoopTimes = ctx.co1Opt * CO0_LOOP_TIMES;
            coPerReg = BLOCK_L0_N / CO0_LOOP_TIMES;
        }
    }

    __aicore__ inline void TransND2NZ(const typename Intf::ContextType &ctx)
    {
        if (unlikely(ctx.groupOptIter == ctx.vecId)) {
            SetIndex();
        }

        // Due to the VF compile, reusing code it's bound to make scalar increase.
        if constexpr (Intf::formatC == ConvFormat::NCDHW) {
            TransNCDHW2NZ();
        } else {
            TransNCHW2NZ();
        }
    }

private:
    __aicore__ inline void SetIndex(const typename Intf::ContextType &ctx)
    {
        // [zj]ToDo: kernel
        // IndexT curValue = 0;
        // for (uint8_t idx = 0; idx < Intf::k0; ++idx) {
        //     indexTensor.SetValue(idx, curValue);
        //     curValue += ctx.convTiling->kernelHxkernelWxkernelD;
        // }
        // event_t eventId = static_cast<event_t>(ctx.pipe->FetchEventID(HardEvent::S_V));
        // SetFlag<HardEvent::S_V>(eventId);
        // WaitFlag<HardEvent::S_V>(eventId);

        // __local_mem__ IndexT *indexAddr = (__local_mem__ IndexT *)indexTensor.GetPhyAddr();
        // uint16_t repeatTimes = static_cast<uint16_t>(REG_SIZE / sizeof(IndexT) / Intf::k0 - 1);
        // uint8_t dstOffset = Intf::k0;
        // uint8_t elesPerRepeat = Intf::k0;
        // uint32_t maskL = Intf::k0;
        // IndexT nStride = static_cast<IndexT>(ctx.kUbSize);

        // __VEC_SCOPE__
        // {
        //     MicroAPI::RegTensor<IndexT> indexReg;
        //     MicroAPI::DataCopy<IndexT>(indexReg, indexAddr);
        //     MicroAPI::MaskReg maskReg = MicroAPI::UpdateMask<IndexT>(maskL);

        //     for (uint16_t repeat = 0; repeat < repeatTimes; ++repeat) {
        //         MicroAPI::Adds<IndexT, IndexT>(indexReg, indexReg, nStride, maskReg);
        //         MicroAPI::DataCopy<IndexT>(indexAddr + dstOffset, indexReg, maskReg);
        //         dstOffset += elesPerRepeat;
        //     }
        // }
    }

    __aicore__ inline void TransNCHW2NZ(const typename Intf::ContextType &ctx)
    {
        // [zj]ToDo: kernel
        // uint16_t ciLoopTimes = ctx.ci1Opt;
        // uint16_t coLoopTimes = coOptLoopTimes;
        // uint16_t khkwLoopTimes = ctx.convTiling->kernelHxkernelW;
        // uint32_t srcCiStride = ctx.convTiling->kernelHxkernelW * Intf::k0;
        // uint32_t srcCoStride = coPerReg * ctx.kUbSize;
        // uint32_t dstCiStride = ctx.convTiling->kernelHxkernelW * Intf::k0 * ctx.coOptAlign;
        // uint32_t dstKhKwStride = Intf::k0 * ctx.coOptAlign;
        // uint32_t dstCoStride = coPerReg * Intf::k0;

        // __VEC_SCOPE__
        // {
        //     MicroAPI::RegTensor<RegT> gatherReg;
        //     MicroAPI::RegTensor<IndexT> indexReg;
        //     MicroAPI::MaskReg gatherMaskReg = MicroAPI::CreateMask<RegT, MicroAPI::MaskPattern::ALL>();
        //     MicroAPI::MaskReg vstsMaskReg;
        //     if constexpr (Intf::isQuantScene) {
        //         vstsMaskReg = MicroAPI::CreateMask<DstT, MicroAPI::MaskPattern::H>();
        //     } else {
        //         vstsMaskReg = MicroAPI::CreateMask<DstT, MicroAPI::MaskPattern::ALL>();
        //     }

        //     __local_mem__ SrcT *srcAddr = (__local_mem__ SrcT *)ndTensor.GetPhyAddr();
        //     __local_mem__ DstT *dstAddr = (__local_mem__ DstT *)ctx.nzTensor.GetPhyAddr();
        //     __local_mem__ IndexT *indexAddr = (__local_mem__ IndexT *)indexTensor.GetPhyAddr();

        //     MicroAPI::DataCopy<IndexT>(indexReg, indexAddr);

        //     for (uint16_t ci1OptIndex = 0; ci1OptIndex < ciLoopTimes; ++ci1OptIndex) {
        //         for (uint16_t khkwIndex = 0; khkwIndex < khkwLoopTimes; ++khkwIndex) {
        //             for (uint16_t coOptIndex = 0; coOptIndex < coLoopTimes; ++coOptIndex) {
        //                 uint32_t srcOffset = ci1OptIndex * srcCiStride + khkwIndex + coOptIndex * srcCoStride;
        //                 uint32_t dstOffset = ci1OptIndex * dstCiStride + khkwIndex * dstKhKwStride +
        //                                      coOptIndex * dstCoStride;

        //                 MicroAPI::DataCopyGather<RegT, SrcT, IndexT>(
        //                     gatherReg, srcAddr + srcOffset, indexReg, gatherMaskReg);

        //                 if constexpr (Intf::isQuantScene) {
        //                     // Remove the higher zeros of the int16_t data gathered by the Micro Gather instr
        //                     MicroAPI::Pack<uint8_t, RegT, MicroAPI::HighLowPart::LOWEST>(
        //                         (MicroAPI::RegTensor<uint8_t> &)gatherReg, gatherReg);
        //                     MicroAPI::DataCopy<DstT>(
        //                         dstAddr + dstOffset, (MicroAPI::RegTensor<DstT> &)gatherReg, vstsMaskReg);
        //                 } else {
        //                     MicroAPI::DataCopy<DstT>(
        //                         dstAddr + dstOffset, (MicroAPI::RegTensor<DstT> &)gatherReg, vstsMaskReg);
        //                 }
        //             }
        //         }
        //     }
        // }
    }

    __aicore__ inline void TransNCDHW2NZ(const typename Intf::ContextType &ctx)
    {
        // [zj]ToDo: kernel
        // uint16_t kdLoopTimes = ctx.convTiling->kernelD;
        // uint16_t ciLoopTimes = ctx.ci1Opt;
        // uint16_t coLoopTimes = coOptLoopTimes;
        // uint16_t khkwLoopTimes = ctx.convTiling->kernelHxkernelW;
        // uint32_t srcCiStride = ctx.convTiling->kernelHxkernelWxkernelD * Intf::k0;
        // uint32_t srcCoStride = coPerReg * ctx.kUbSize;
        // uint32_t dstCiStride = ctx.convTiling->kernelHxkernelW * Intf::k0 * ctx.coOptAlign;
        // uint32_t dstKhKwStride = Intf::k0 * ctx.coOptAlign;
        // uint32_t dstCoStride = coPerReg * Intf::k0;
        // uint32_t srcKdStride = ctx.convTiling->kernelHxkernelW;
        // uint32_t dstKdStride = ctx.ci1Opt * dstCiStride;

        // __VEC_SCOPE__
        // {
        //     MicroAPI::RegTensor<RegT> gatherReg;
        //     MicroAPI::RegTensor<IndexT> indexReg;
        //     MicroAPI::MaskReg gatherMaskReg = MicroAPI::CreateMask<RegT, MicroAPI::MaskPattern::ALL>();
        //     MicroAPI::MaskReg vstsMaskReg;
        //     if constexpr (Intf::isQuantScene) {
        //         vstsMaskReg = MicroAPI::CreateMask<DstT, MicroAPI::MaskPattern::H>();
        //     } else {
        //         vstsMaskReg = MicroAPI::CreateMask<DstT, MicroAPI::MaskPattern::ALL>();
        //     }

        //     __local_mem__ SrcT *srcAddr = (__local_mem__ SrcT *)ndTensor.GetPhyAddr();
        //     __local_mem__ DstT *dstAddr = (__local_mem__ DstT *)ctx.nzTensor.GetPhyAddr();
        //     __local_mem__ IndexT *indexAddr = (__local_mem__ IndexT *)indexTensor.GetPhyAddr();

        //     MicroAPI::DataCopy<IndexT>(indexReg, indexAddr);

        //     for (uint16_t kdIndex = 0; kdIndex < kdLoopTimes; ++kdIndex) {
        //         for (uint16_t ci1OptIndex = 0; ci1OptIndex < ciLoopTimes; ++ci1OptIndex) {
        //             for (uint16_t khkwIndex = 0; khkwIndex < khkwLoopTimes; ++khkwIndex) {
        //                 for (uint16_t coOptIndex = 0; coOptIndex < coLoopTimes; ++coOptIndex) {
        //                     uint32_t srcOffset = kdIndex * srcKdStride + ci1OptIndex * srcCiStride + khkwIndex +
        //                                          coOptIndex * srcCoStride;
        //                     uint32_t dstOffset = kdIndex * dstKdStride + ci1OptIndex * dstCiStride +
        //                                          khkwIndex * dstKhKwStride + coOptIndex * dstCoStride;

        //                     MicroAPI::DataCopyGather<RegT, SrcT, IndexT>(
        //                         gatherReg, srcAddr + srcOffset, indexReg, gatherMaskReg);

        //                     if constexpr (Intf::isQuantScene) {
        //                         // Remove the higher zeros of the int16_t data gathered by the Micro Gather instr
        //                         MicroAPI::Pack<uint8_t, RegT, MicroAPI::HighLowPart::LOWEST>(
        //                             (MicroAPI::RegTensor<uint8_t> &)gatherReg, gatherReg);
        //                         MicroAPI::DataCopy<DstT>(
        //                             dstAddr + dstOffset, (MicroAPI::RegTensor<DstT> &)gatherReg, vstsMaskReg);
        //                     } else {
        //                         MicroAPI::DataCopy<DstT>(
        //                             dstAddr + dstOffset, gatherReg, vstsMaskReg);
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }
    }

private:
    // typename Intf::ContextType *ctx_;

    using SrcT = typename Conditional<Intf::isQuantScene, int8_t, typename Intf::WeightT>::type;
    using DstT = typename Conditional<Intf::isQuantScene, int8_t, typename Intf::WeightT>::type;
    using RegT = typename Conditional<Intf::isQuantScene, int16_t, typename Intf::WeightT>::type;
    using IndexT = typename Conditional<
        AscendC::IsSameType<typename Intf::WeightT, float>::value, uint32_t, uint16_t>::type;

    LocalTensor<SrcT> ndTensor;
    LocalTensor<IndexT> indexTensor;

    uint16_t coOptLoopTimes = 0;
    uint16_t coPerReg = 0;
};

template <class Intf>
class LoadUB2L1Tools {
public:
    __aicore__ inline LoadUB2L1Tools()
    {
    }

    __aicore__ inline void SetParams(const typename Intf::ContextType &ctx)
    {
        // ctx_ = ctx;
    }

    __aicore__ inline void LoadUB2L1(const typename Intf::ContextType &ctx)
    {
        if constexpr (Intf::formatC == ConvFormat::NCDHW) {
            SetCopyParamsNCDHW();
        } else {
            SetCopyParamsNCHW();
        }

        if constexpr (!Intf::bL1DBFlag) {
            DataCopy<typename Intf::WeightT>(ctx.bl1, ctx.nzTensor[srcOffset], copyParams);
        } else {
            DataCopy<typename Intf::WeightT>(ctx.bl1[ctx.pingPongFlag * ctx.bL1SpaceSize],
                ctx.nzTensor[srcOffset], copyParams);
            ctx.pingPongFlag ^= 1;
        }
    }

private:
    __aicore__ inline void SetCopyParamsNCHW(const typename Intf::ContextType &ctx)
    {
        if (unlikely(ctx.groupOptIter == ctx.vecId)) {
            copyParams.blockCount = ctx.convTiling->kBL1 / Intf::k0;
            copyParams.blockLen = ctx.convTiling->nBL1;
            copyParams.srcStride = ctx.co1Opt * BLOCK_L0_N - ctx.convTiling->nBL1;
        }

        srcOffset = (ctx.coStartPos + ctx.nBL1Iter * ctx.convTiling->nBL1) * Intf::k0 +
                    ctx.kBL1Iter * ctx.convTiling->kBL1 * ctx.coOptAlign;
    }

    __aicore__ inline void SetCopyParamsNCDHW(const typename Intf::ContextType &ctx)
    {
        if (unlikely(ctx.loadUB2L1Iter == 0)) {
            copyParams.blockLen = ctx.convTiling->nBL1;
            copyParams.srcStride = ctx.co1Opt * BLOCK_L0_N - ctx.convTiling->nBL1;
            kOffset = ctx.bL1Dk * ctx.bL1Cin * ctx.convTiling->kernelHxkernelW;
        }

        uint64_t currentBL1Dk = IsKBL1Tail() ? ctx.bL1DkTail : ctx.bL1Dk;
        uint64_t currentBL1Cin1 = IsKBL1Tail() ? ctx.bL1CinTail : ctx.bL1Cin;
        copyParams.blockCount = currentBL1Dk * CeilDIV(currentBL1Cin1, Intf::k0) *
                                ctx.convTiling->kernelHxkernelW;

        srcOffset = (ctx.coStartPos + ctx.nBL1Iter * ctx.convTiling->nBL1) * Intf::k0 +
                    ctx.kBL1Iter * kOffset * ctx.coOptAlign;
    }

    __aicore__ inline bool IsKBL1Tail(const typename Intf::ContextType &ctx)
    {
        if (ctx.bL1CinLoadNum == 1) {
            return ctx.kBL1Iter == ctx.maxKBL1Iter;
        } else {
            return (ctx.kBL1Iter + 1) % ctx.bL1CinLoadNum == 0;
        }
    }

private:
    // typename Intf::ContextType *ctx_;

    DataCopyParams copyParams;
    uint64_t kOffset = 0;
    uint64_t srcOffset = 0;
};

};

#endif // CONV_INSTR_OPT_GROUP_IMPL_H