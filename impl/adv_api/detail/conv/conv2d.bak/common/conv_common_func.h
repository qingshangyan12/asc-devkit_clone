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
 * \file conv_common_func.h
 * \brief
 */

#ifndef CONV_COMMON_FUNC_H
#define CONV_COMMON_FUNC_H

#include "conv_config.h"
#include "conv_framework_util.h"
#include "conv_iterate_impl.h"
#include "conv_opt_group_impl.h"
#include "conv_util.h"

namespace ConvFunc {
using namespace conv;

CONV_DECLARE_REG_IMPL(SetFmap);
CONV_DECLARE_REG_IMPL(SetWeight);
CONV_DECLARE_REG_IMPL(SetBias);
CONV_DECLARE_REG_IMPL(SetQuantPre);
CONV_DECLARE_REG_IMPL(SetReluPre);
CONV_DECLARE_REG_IMPL(SetEltAntiQuant);
CONV_DECLARE_REG_IMPL(SetQuantPost);
CONV_DECLARE_REG_IMPL(SetQuantFixpipe);
CONV_DECLARE_REG_IMPL(SetEltwise);
CONV_DECLARE_REG_IMPL(SetSingleBatchNum);
CONV_DECLARE_REG_IMPL(IterateAll);
CONV_DECLARE_REG_IMPL(GetTensorC);
CONV_DECLARE_REG_IMPL(End);

using TypeFalse = struct {
    __uint128_t _[1024];
};

template <class Intf, uint32_t ImplType>
struct SetFmap {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::FmapT> &fmap)
    {
        if ASCEND_IS_AIC {
            self->ctx.agm.SetGlobalBuffer(fmap.GetPhyAddr(0), fmap.GetSize());
            self->ctx.isFirstIterate = true;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetWeight {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::WeightT> &weight)
    {
        self->ctx.bgm.SetGlobalBuffer(weight.GetPhyAddr(0), weight.GetSize());
        self->ctx.isFirstIterate = true;
    }
};

template <class Intf, uint32_t ImplType>
struct SetBias {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::BiasT> &bias)
    {
        if ASCEND_IS_AIC {
            self->ctx.biasgm.SetGlobalBuffer(bias.GetPhyAddr(0), bias.GetSize());
            self->ctx.isFirstIterate = true;
            self->ctx.enableBias = true;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetQuantPre {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::QuantPreT> &quantPre)
    {
        if ASCEND_IS_AIC {
            self->ctx.quantPreGm.SetGlobalBuffer(quantPre.GetPhyAddr(0), quantPre.GetSize());
            self->ctx.isFirstIterate = true;
            self->ctx.enableQuantPre = true;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetReluPre {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::ReluPreT> &reluPre)
    {
        if ASCEND_IS_AIC {
            self->ctx.reluPreGm.SetGlobalBuffer(reluPre.GetPhyAddr(0), reluPre.GetSize());
            self->ctx.isFirstIterate = true;
            self->ctx.enableReluPre = true;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetEltAntiQuant {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::EltAntiQuantT> &eltAntiQuant)
    {
        if ASCEND_IS_AIC {
            self->ctx.eltAntiQuantGm.SetGlobalBuffer(eltAntiQuant.GetPhyAddr(0), eltAntiQuant.GetSize());
            self->ctx.isFirstIterate = true;
            self->ctx.enableEltAntiQuant = true;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetQuantPost {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::QuantPostT> &quantPost)
    {
        if ASCEND_IS_AIC {
            self->ctx.quantPostGm.SetGlobalBuffer(quantPost.GetPhyAddr(0), quantPost.GetSize());
            self->ctx.isFirstIterate = true;
            self->ctx.enableQuantPost = true;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetQuantFixpipe {
    static __aicore__ inline void call(Intf *self, const uint64_t &quantPreMode, const uint64_t &reluPreMode,
                                       const uint64_t &clipReluPreMode, const uint64_t &quantPostMode,
                                       const uint64_t &reluPostMode, const uint64_t &eltwiseOp,
                                       const uint64_t &eltwiseAntiqEn, const uint64_t &eltwiseBrcEn,
                                       const uint64_t &quantPreSpr, const uint64_t &reluPreSpr,
                                       const uint64_t &antiQuantSpr, const uint64_t &quantPostSpr)
    {
        if ASCEND_IS_AIC {
            self->ctx.quantPreMode = quantPreMode;
            self->ctx.reluPreMode = reluPreMode;
            self->ctx.clipReluPreMode = clipReluPreMode;
            self->ctx.quantPostMode = quantPostMode;
            self->ctx.reluPostMode = reluPostMode;
            self->ctx.eltwiseOp = eltwiseOp;
            self->ctx.eltwiseAntiqEn = eltwiseAntiqEn;
            self->ctx.eltwiseBrcEn = eltwiseBrcEn;
            self->ctx.quantPreSpr = quantPreSpr;
            self->ctx.reluPreSpr = reluPreSpr;
            self->ctx.antiQuantSpr = antiQuantSpr;
            self->ctx.quantPostSpr = quantPostSpr;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetEltwise {
    static __aicore__ inline void call(Intf *self, const GlobalTensor<typename Intf::EltwiseT> &eltwise)
    {
        if ASCEND_IS_AIC {
            self->ctx.eltwiseGm.SetGlobalBuffer(eltwise.GetPhyAddr(0), eltwise.GetSize());
            self->ctx.isFirstIterate = true;
            self->ctx.enableEltwise = true;
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetSingleBatchNum {
    static __aicore__ inline void call(Intf *self, uint64_t singleCoreBatch,
                                       uint64_t fmapOneBatchSize, uint64_t outputOneBatchSize)
    {
        self->ctx.ddr2l1LoopBatch = singleCoreBatch;
        if ASCEND_IS_AIC {
            self->ctx.fmapOneBatchSize = fmapOneBatchSize;
            self->ctx.outputOneBatchSize = outputOneBatchSize;
            if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                OptGroupCalcBL1LoadTimes<Intf>(self);
            }
        } else {
            if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                OptGroupInitIterValue<Intf>(self);
                if (self->ctx.vecId == 1) {
                    OptGroupCalcBL1LoadTimes<Intf>(self);
                }
            }
        }
    }
};

template <class Intf, uint32_t ImplType>
struct GetTensorC {
    template <bool sync = true>
    static __aicore__ inline void call(
        Intf *self, const GlobalTensor<typename Intf::OutputT> &output, bool enSequentialWrite = false)
    {
        if ASCEND_IS_AIC {
            self->ctx.copyOutIns.CopyOut(output);

#if defined(__DAV_L510__)
            self->ctx.queueCL1.FreeTensor(self->ctx.cl1);
#else
            self->ctx.queueCL0.FreeTensor(self->ctx.cl0);
#endif

            if (self->ctx.enableQuantPre && !self->ctx.convTiling->fixpParamsFullLoadFlag) {
                self->ctx.queueQuantPreL1.FreeTensor(self->ctx.quantPreL1);
            }
            if (self->ctx.enableReluPre && !self->ctx.convTiling->fixpParamsFullLoadFlag) {
                self->ctx.queueReluPreL1.FreeTensor(self->ctx.reluPreL1);
            }
            if (self->ctx.enableEltAntiQuant && !self->ctx.convTiling->fixpParamsFullLoadFlag) {
                self->ctx.queueEltAntiQuantL1.FreeTensor(self->ctx.eltAntiQuantL1);
            }
            if (self->ctx.enableQuantPost && !self->ctx.convTiling->fixpParamsFullLoadFlag) {
                self->ctx.queueQuantPostL1.FreeTensor(self->ctx.quantPostL1);
            }
            if (self->ctx.enableEltwise && (!self->ctx.eltwiseBrcEn || !self->ctx.convTiling->fixpParamsFullLoadFlag)) {
                self->ctx.queueEltwiseL1.FreeTensor(self->ctx.eltwiseL1);
            }
        }
    }
};

template <class Intf, uint32_t ImplType>
struct IterateAll {
    template <bool sync = true>
    static __aicore__ inline bool call(
        Intf *self, const GlobalTensor<typename Intf::OutputT> &output, bool enPartialSum = false)
    {
        if ASCEND_IS_AIC {
            if (self->ctx.convTiling->biasFullLoadFlag && self->ctx.enableBias) {
                self->ctx.biasL1 = self->ctx.queueBiasL1.template AllocTensor<typename Intf::BiasT>();
                self->ctx.loadBiasL1Ins.LoadChannelWiseL1FullLoad(self->ctx.biasL1, self->ctx.biasgm,
                                                                  self->ctx.singleCoreCo, 0);
                self->ctx.queueBiasL1.EnQue(self->ctx.biasL1);
                self->ctx.biasL1 = self->ctx.queueBiasL1.template DeQue<typename Intf::BiasT>();
                // DumpAccChkPoint(self->ctx.biasL1, 0, 0, self->ctx.singleCoreCo);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableQuantPre) {
                if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                self->ctx.quantPreL1 = self->ctx.queueQuantPreL1.template AllocTensor<typename Intf::QuantPreT>();
                self->ctx.loadQuantPreL1Ins.LoadChannelWiseL1FullLoad(self->ctx.quantPreL1, self->ctx.quantPreGm,
                                                                      self->ctx.singleCoreCo, 0);
                self->ctx.queueQuantPreL1.EnQue(self->ctx.quantPreL1);
                self->ctx.quantPreL1 = self->ctx.queueQuantPreL1.template DeQue<typename Intf::QuantPreT>();
                // // DumpAccChkPoint(self->ctx.quantPreL1.ReinterpretCast<uint32_t>(), 1, 0, self->ctx.singleCoreCo * 2);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableReluPre) {
                if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                self->ctx.reluPreL1 = self->ctx.queueReluPreL1.template AllocTensor<typename Intf::ReluPreT>();
                self->ctx.loadReluPreL1Ins.LoadChannelWiseL1FullLoad(self->ctx.reluPreL1, self->ctx.reluPreGm,
                                                                     self->ctx.singleCoreCo, 0);
                self->ctx.queueReluPreL1.EnQue(self->ctx.reluPreL1);
                self->ctx.reluPreL1 = self->ctx.queueReluPreL1.template DeQue<typename Intf::ReluPreT>();
                // DumpAccChkPoint(self->ctx.reluPreL1, 2, 0, self->ctx.singleCoreCo);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableEltAntiQuant) {
                if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                self->ctx.eltAntiQuantL1 =
                    self->ctx.queueEltAntiQuantL1.template AllocTensor<typename Intf::EltAntiQuantT>();
                self->ctx.loadEltAntiQuantL1Ins.LoadChannelWiseL1FullLoad(self->ctx.eltAntiQuantL1,
                                                                          self->ctx.eltAntiQuantGm,
                                                                          self->ctx.singleCoreCo, 0);
                self->ctx.queueEltAntiQuantL1.EnQue(self->ctx.eltAntiQuantL1);
                self->ctx.eltAntiQuantL1 = self->ctx.queueEltAntiQuantL1.template DeQue<typename Intf::EltAntiQuantT>();
                // DumpAccChkPoint(self->ctx.eltAntiQuantL1, 3, 0, self->ctx.singleCoreCo);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableQuantPost) {
                if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                self->ctx.quantPostL1 = self->ctx.queueQuantPostL1.template AllocTensor<typename Intf::QuantPostT>();
                self->ctx.loadQuantPostL1Ins.LoadChannelWiseL1FullLoad(self->ctx.quantPostL1, self->ctx.quantPostGm,
                                                                       self->ctx.singleCoreCo, 0);
                self->ctx.queueQuantPostL1.EnQue(self->ctx.quantPostL1);
                self->ctx.quantPostL1 = self->ctx.queueQuantPostL1.template DeQue<typename Intf::QuantPostT>();
                // // DumpAccChkPoint(self->ctx.quantPostL1.ReinterpretCast<uint32_t>(), 4, 0, self->ctx.singleCoreCo * 2);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableEltwise && self->ctx.eltwiseBrcEn) {
                if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                self->ctx.eltwiseL1 = self->ctx.queueEltwiseL1.template AllocTensor<typename Intf::EltwiseT>();
                self->ctx.loadEltwiseL1Ins.LoadChannelWiseL1FullLoad(self->ctx.eltwiseL1, self->ctx.eltwiseGm,
                                                                     self->ctx.singleCoreCo, 0);
                self->ctx.queueEltwiseL1.EnQue(self->ctx.eltwiseL1);
                self->ctx.eltwiseL1 = self->ctx.queueEltwiseL1.template DeQue<typename Intf::EltwiseT>();
                // DumpAccChkPoint(self->ctx.eltwiseL1, 5, 0, self->ctx.singleCoreCo);
            }
        }

        while (Iterate<Intf, ImplType>::call(self, enPartialSum)) {
            GetTensorC<Intf, ImplType>::call(self, output);
        }

        if ASCEND_IS_AIC {
            if (self->ctx.convTiling->biasFullLoadFlag && self->ctx.enableBias) {
                self->ctx.queueBiasL1.FreeTensor(self->ctx.biasL1);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableQuantPre) {
                self->ctx.queueQuantPreL1.FreeTensor(self->ctx.quantPreL1);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableReluPre) {
                self->ctx.queueReluPreL1.FreeTensor(self->ctx.reluPreL1);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableEltAntiQuant) {
                self->ctx.queueEltAntiQuantL1.FreeTensor(self->ctx.eltAntiQuantL1);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableQuantPost) {
                self->ctx.queueQuantPostL1.FreeTensor(self->ctx.quantPostL1);
            }
            if (self->ctx.convTiling->fixpParamsFullLoadFlag && self->ctx.enableEltwise && self->ctx.eltwiseBrcEn) {
                self->ctx.queueEltwiseL1.FreeTensor(self->ctx.eltwiseL1);
            }

            self->ctx.isFirstIterate = true;
        }

        return false;
    }
};

template <class Intf, uint32_t ImplType>
struct End {
    static __aicore__ inline void call(Intf *self)
    {
        if ASCEND_IS_AIC {
            self->ctx.queueAL1.FreeAllEvent();
            self->ctx.queueBL1.FreeAllEvent();
            self->ctx.queueBiasL1.FreeAllEvent();
            self->ctx.queueQuantPreL1.FreeAllEvent();
            self->ctx.queueReluPreL1.FreeAllEvent();
            self->ctx.queueEltAntiQuantL1.FreeAllEvent();
            self->ctx.queueQuantPostL1.FreeAllEvent();
            self->ctx.queueEltwiseL1.FreeAllEvent();
            self->ctx.queueBiasBT.FreeAllEvent();
            self->ctx.queueQuantPreFB.FreeAllEvent();
            self->ctx.queueReluPreFB.FreeAllEvent();
            self->ctx.queueEltAntiQuantFB.FreeAllEvent();
            self->ctx.queueQuantPostFB.FreeAllEvent();
            self->ctx.queueCL1.FreeAllEvent();
            self->ctx.queueCL0.FreeAllEvent();
        }
    }
};

template <class Intf>
__aicore__ inline void InitBufferWithDoubleBuf(Intf *self, int64_t cl0Spacesize, uint64_t bl1Spacesize)
{
    // [zj]L0aL0bTQueLocalTensoreventid
    self->ctx.pipe->InitBuffer(self->ctx.l0aBuf, L0A_SIZE);
    self->ctx.pipe->InitBuffer(self->ctx.l0bBuf, L0B_SIZE);
    self->ctx.wholeAl0Tensor = self->ctx.l0aBuf.template Get<typename Intf::FmapT>();
    self->ctx.wholeBl0Tensor = self->ctx.l0bBuf.template Get<typename Intf::WeightT>();

    int8_t cl0db = (self->ctx.convTiling->pBufferFlag & 0x04) >> 2;
    int8_t al1db = (self->ctx.convTiling->pBufferFlag & 0x08) >> 3;
    int8_t bl1db = (self->ctx.convTiling->pBufferFlag & 0x10) >> 4;
    printf("cl0db: %d, al1db: %d, bl1db: %d\n", cl0db, al1db, Intf::bL1DBFlag);

#if defined(__DAV_L510__)
    if (!cl0db) {
        self->ctx.pipe->InitBuffer(self->ctx.queueCL1, 1, cl0Spacesize * Intf::sizeOfOutput);
    } else {
        self->ctx.pipe->InitBuffer(self->ctx.queueCL1, DOUBLE_BUF, cl0Spacesize * Intf::sizeOfOutput);
    }
#else
    if (!cl0db) {
        self->ctx.pipe->InitBuffer(self->ctx.queueCL0, 1, cl0Spacesize * Intf::sizeOfL0c);
    } else {
        self->ctx.pipe->InitBuffer(self->ctx.queueCL0, DOUBLE_BUF, cl0Spacesize * Intf::sizeOfL0c);
    }
#endif

    // [zj]aL1SpaceSizesizeOfFmap
    if (!al1db) {
        self->ctx.pipe->InitBuffer(self->ctx.queueAL1, 1, self->ctx.convTiling->aL1SpaceSize);
    } else {
        self->ctx.pipe->InitBuffer(self->ctx.queueAL1, DOUBLE_BUF, self->ctx.convTiling->aL1SpaceSize);
    }

    // [zj]ToDodb
    if constexpr (!Intf::bL1DBFlag) {
        self->ctx.pipe->InitBuffer(
            self->ctx.queueBL1, 1, bl1Spacesize * Intf::sizeOfWeight);
    } else {
        self->ctx.pipe->InitBuffer(
            self->ctx.queueBL1, DOUBLE_BUF, bl1Spacesize * Intf::sizeOfWeight);
    }
}

template <class Intf>
__aicore__ inline void InitBuffer(Intf *self)
{
    uint64_t cl0Spacesize = self->ctx.convTiling->mStep * self->ctx.convTiling->nL0;
    uint64_t bl1Spacesize = self->ctx.convTiling->nBL1 * self->ctx.convTiling->kBL1;

    // [zj]l0al0bl0cl1al1bLocalTensor
    InitBufferWithDoubleBuf(self, cl0Spacesize, bl1Spacesize);

    int8_t cl0db = (self->ctx.convTiling->pBufferFlag & 0x04) >> 2;
    uint8_t bufferNum = cl0db ? DOUBLE_BUF : 1;

    // [zj]l1biasl0biasLocalTensor
    if (self->ctx.convTiling->hasBias) {
        uint64_t biasl1Spacesize = self->ctx.convTiling->biasFullLoadFlag
                                       ? AlignB(self->ctx.singleCoreCo * Intf::sizeOfBias,
                                                BLOCK_L0_N * Intf::sizeOfBias)
                                       : self->ctx.convTiling->nL0 * Intf::sizeOfBias;
        uint64_t biasBTSpacesize = self->ctx.convTiling->nL0 * Intf::sizeOfL0c;
        // [zj]ToDo
#if defined(__DAV_L510__)
        biasBTSpacesize = 1 * 1024 / static_cast<uint64_t>(bufferNum);
#endif
        printf("biasBTSpacesize: %ld\n", biasBTSpacesize);
        self->ctx.pipe->InitBuffer(self->ctx.queueBiasL1, 1, AlignB(biasl1Spacesize, C0_SIZE));
        self->ctx.pipe->InitBuffer(self->ctx.queueBiasBT, bufferNum, AlignB(biasBTSpacesize, BT_SIZE));
    }
    // [zj]ToDo
    if (self->ctx.convTiling->hasQuantPre) {
        uint64_t quantPreL1SpaceSize = self->ctx.convTiling->fixpParamsFullLoadFlag
                                           ? AlignB(self->ctx.singleCoreCo * Intf::sizeOfQuantPre,
                                                    BLOCK_L0_N * Intf::sizeOfQuantPre)
                                           : self->ctx.convTiling->nL0 * Intf::sizeOfQuantPre;
        self->ctx.pipe->InitBuffer(self->ctx.queueQuantPreL1, 1, AlignB(quantPreL1SpaceSize, C0_SIZE));
#if defined(__DAV_L510__)
        uint64_t quantPreFBSpacesize = self->ctx.convTiling->nL0 * Intf::sizeOfQuantPre;
        // [zj]ToDo
        quantPreFBSpacesize = 2 * 1024 / static_cast<uint64_t>(bufferNum);
        printf("quantPreFBSpacesize: %ld\n", quantPreFBSpacesize);
        self->ctx.pipe->InitBuffer(self->ctx.queueQuantPreFB, bufferNum, AlignB(quantPreFBSpacesize, FB_QUANT_PRE_SIZE));
#endif
    }
    if (self->ctx.convTiling->hasReluPre) {
        uint64_t reluPreL1SpaceSize = self->ctx.convTiling->fixpParamsFullLoadFlag
                                          ? AlignB(self->ctx.singleCoreCo * Intf::sizeOfReluPre,
                                                   BLOCK_L0_N * Intf::sizeOfReluPre)
                                          : self->ctx.convTiling->nL0 * Intf::sizeOfReluPre;
        self->ctx.pipe->InitBuffer(self->ctx.queueReluPreL1, 1, AlignB(reluPreL1SpaceSize, C0_SIZE));
#if defined(__DAV_L510__)
        uint64_t reluPreFBSpacesize = self->ctx.convTiling->nL0 * Intf::sizeOfReluPre;
        // [zj]ToDo
        reluPreFBSpacesize = 1 * 1024 / static_cast<uint64_t>(bufferNum);
        printf("reluPreFBSpacesize: %ld\n", reluPreFBSpacesize);
        self->ctx.pipe->InitBuffer(self->ctx.queueReluPreFB, bufferNum, AlignB(reluPreFBSpacesize, FB_RELU_PRE_SIZE));
#endif
    }
    if (self->ctx.convTiling->hasEltAntiQuant) {
        uint64_t eltAntiQuantL1SpaceSize = self->ctx.convTiling->fixpParamsFullLoadFlag
                                               ? AlignB(self->ctx.singleCoreCo * Intf::sizeOfEltAntiQuant,
                                                        BLOCK_L0_N * Intf::sizeOfEltAntiQuant)
                                               : self->ctx.convTiling->nL0 * Intf::sizeOfEltAntiQuant;
        self->ctx.pipe->InitBuffer(self->ctx.queueEltAntiQuantL1, 1, AlignB(eltAntiQuantL1SpaceSize, C0_SIZE));
#if defined(__DAV_L510__)
        uint64_t eltAntiQuantFBSpacesize = self->ctx.convTiling->nL0 * Intf::sizeOfEltAntiQuant;
        // [zj]ToDo
        eltAntiQuantFBSpacesize = 1 * 1024 / static_cast<uint64_t>(bufferNum);
        printf("eltAntiQuantFBSpacesize: %ld\n", eltAntiQuantFBSpacesize);
        self->ctx.pipe->InitBuffer(self->ctx.queueEltAntiQuantFB, bufferNum,
                                  AlignB(eltAntiQuantFBSpacesize, FB_ELT_ANTIQ_SIZE));
#endif
    }
    if (self->ctx.convTiling->hasQuantPost) {
        uint64_t quantPostL1SpaceSize = self->ctx.convTiling->fixpParamsFullLoadFlag
                                            ? AlignB(self->ctx.singleCoreCo * Intf::sizeOfQuantPost,
                                                     BLOCK_L0_N * Intf::sizeOfQuantPost)
                                            : self->ctx.convTiling->nL0 * Intf::sizeOfQuantPost;
        self->ctx.pipe->InitBuffer(self->ctx.queueQuantPostL1, 1, AlignB(quantPostL1SpaceSize, C0_SIZE));
#if defined(__DAV_L510__)
        uint64_t quantPostFBSpacesize = self->ctx.convTiling->nL0 * Intf::sizeOfQuantPost;
        // [zj]ToDo
        quantPostFBSpacesize = 2 * 1024 / static_cast<uint64_t>(bufferNum);
        printf("quantPostFBSpacesize: %ld\n", quantPostFBSpacesize);
        self->ctx.pipe->InitBuffer(self->ctx.queueQuantPostFB, bufferNum,
                                  AlignB(quantPostFBSpacesize, FB_QUANT_POST_SIZE));
#endif
    }
    if (self->ctx.convTiling->hasEltwise) {
        uint64_t eltwiseL1SpaceSize = 0;
        if (self->ctx.eltwiseBrcEn) {
            eltwiseL1SpaceSize = self->ctx.convTiling->fixpParamsFullLoadFlag
                                     ? AlignB(self->ctx.singleCoreCo * Intf::sizeOfEltwise,
                                              BLOCK_L0_N * Intf::sizeOfEltwise)
                                     : self->ctx.convTiling->nL0 * Intf::sizeOfEltwise;
        } else {
            eltwiseL1SpaceSize = self->ctx.convTiling->mStep * self->ctx.convTiling->nL0 * Intf::sizeOfEltwise;
        }
        self->ctx.pipe->InitBuffer(self->ctx.queueEltwiseL1, bufferNum, AlignB(eltwiseL1SpaceSize, C0_SIZE));
    }
}

template <class Intf>
__aicore__ inline void InitHf32Mode(Intf *self)
{
    if (self->ctx.convTiling->hf32Enable) {
        SetHF32Mode(self->ctx.convTiling->hf32Enable);
        SetHF32TransMode(self->ctx.convTiling->hf32TransMode);
    }
}

template <class Intf>
__aicore__ inline void InitSubApiParams(Intf *self)
{
    // Tools
    self->ctx.loadBiasL1Ins.SetParams(self);
    self->ctx.loadQuantPreL1Ins.SetParams(self);
    self->ctx.loadReluPreL1Ins.SetParams(self);
    self->ctx.loadEltAntiQuantL1Ins.SetParams(self);
    self->ctx.loadQuantPostL1Ins.SetParams(self);
    self->ctx.loadEltwiseL1Ins.SetParams(self);
    self->ctx.loadEltwiseNonBrcL1Ins.SetParams(self);
    self->ctx.loadBL1Ins.SetParams(self);
    self->ctx.loadAl1Ins.SetParams(self);
    self->ctx.loadAL0Ins.SetParams(self);
    self->ctx.loadBL0Ins.SetParams(self);
    self->ctx.madIns.SetParams(self);
    self->ctx.loadBiasBTIns.SetParams(self);
    self->ctx.loadQuantPreFBIns.SetParams(self);
    self->ctx.loadReluPreFBIns.SetParams(self);
    self->ctx.loadEltAntiQuantFBIns.SetParams(self);
    self->ctx.loadQuantPostFBIns.SetParams(self);
    self->ctx.copyOutIns.SetParams(self);
}

}  // namespace ConvFunc
#endif