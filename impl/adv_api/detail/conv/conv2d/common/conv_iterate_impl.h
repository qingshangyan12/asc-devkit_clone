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
 * \file conv_iterate_impl.h
 * \brief
 */

#ifndef CONV_ITERATE_IMPL_H
#define CONV_ITERATE_IMPL_H

#include "conv_config.h"
#include "conv_framework_util.h"
#include "conv_iterate_base_impl.h"
#include "conv_iterate_hw_mode_impl.h"
#include "conv_iterate_m_mode_impl.h"
#include "conv_util.h"

namespace ConvFunc {

constexpr uint64_t DST_MEM_BLOCK_PRE_QUANT = 0;
constexpr uint64_t DST_MEM_BLOCK_PRE_RELU = 1;
constexpr uint64_t DST_MEM_BLOCK_POST_RELU = 2;
constexpr uint64_t DST_MEM_BLOCK_POST_QUANT = 3;
constexpr uint64_t DST_MEM_BLOCK_ANTI_QUANT = 4;

using namespace AscendC;
using namespace conv;

CONV_DECLARE_REG_IMPL(Iterate);

template <class Intf, uint32_t ImplType>
struct Iterate {
    template <bool sync = true>
    static __aicore__ inline bool call(Intf *self, bool enPartialSum = false)
    {
        if ASCEND_IS_AIC {
            if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
                return IterateImplMMode(self, enPartialSum);
            } else {
                return IterateImplHWMode(self, enPartialSum);
            }
        } else {
            if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                return OptGroupVecImpl<Intf>(self);
            } else {
                return false;
            }
        }
    }

    static __aicore__ inline bool IterateImplHWMode(Intf *self, bool enPartialSum);
    static __aicore__ inline bool IterateImplMMode(Intf *self, bool enPartialSum);
    static __aicore__ inline void IterateK(Intf *self);
    static __aicore__ inline void ReduceK(Intf *self);
    static __aicore__ inline void ReduceOneK(Intf *self);
    static __aicore__ inline void ReduceKPreload(Intf *self);
    static __aicore__ inline bool UpdateItersByFmap(Intf *self, TempIters& tempIters, bool updateIterByFmapTag);
    static __aicore__ inline bool UpdateItersByWeight(Intf *self, TempIters& tempIters, bool updateIterByFmapTag);
    static __aicore__ inline bool UpdateCommonIters(Intf *self, TempIters& tempIters);
};

template <class Intf, uint32_t ImplType>
__aicore__ void Iterate<Intf, ImplType>::ReduceOneK(Intf *self)
{
    // [zj]ToDo
    if (Intf::mPreLoadAFlag) {
        if (unlikely(self->ctx.loadAL1Flag)) {
            printf("iterate fmap preload setspr\n");
            self->ctx.loadAl1Ins.SetSpr();
            printf("[0]kAL1Iter: %ld, mAL1Iter: %ld, batchIter: %ld\n", self->ctx.kAL1Iter, self->ctx.mAL1Iter, self->ctx.batchIter);
            printf("[0]kAL1Iter: %ld, hoAL1Iter: %ld, woAL1Iter: %ld, batchIter: %ld\n", self->ctx.kAL1Iter, self->ctx.hoAL1Iter, self->ctx.woAL1Iter, self->ctx.batchIter);
            TempIters tempIters;
            if (!UpdateItersByFmap(self, tempIters, true)) {
                printf("iterate fmap preload\n");
                printf("[1]kAL1Iter: %ld, mAL1Iter: %ld, batchIter: %ld\n", tempIters.kAL1Iter, tempIters.mAL1Iter, tempIters.batchIter);
                printf("[1]kAL1Iter: %ld, hoAL1Iter: %ld, woAL1Iter: %ld, batchIter: %ld\n", tempIters.kAL1Iter, tempIters.hoAL1Iter, tempIters.woAL1Iter, tempIters.batchIter);
                if ((Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
                        (tempIters.kAL1Iter != self->ctx.kAL1Iter || tempIters.mAL1Iter != self->ctx.mAL1Iter ||
                        tempIters.batchIter != self->ctx.batchIter)) ||
                    (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
                        (tempIters.hoAL1Iter != self->ctx.hoAL1Iter || tempIters.woAL1Iter != self->ctx.woAL1Iter ||
                        tempIters.mAL1Iter != self->ctx.mAL1Iter || tempIters.batchIter != self->ctx.batchIter))) {
                    bool mAL1UpdateFlag = self->ctx.mAL1UpdateFlag;
                    self->ctx.mAL1UpdateFlag = true;
                    LoadAL1BaseMoudle<Intf>(self, tempIters);
                    self->ctx.mAL1UpdateFlag = mAL1UpdateFlag;
                }
            }
            printf("iterate fmap preload deque\n");
            self->ctx.al1 = self->ctx.queueAL1.template DeQue<typename Intf::FmapT>();
            self->ctx.loadAL1Flag = false;
        }
    } else {
        if (unlikely(self->ctx.loadAL1Flag)) {
            LoadAL1Moudle<Intf>(self);
            self->ctx.al1 = self->ctx.queueAL1.template DeQue<typename Intf::FmapT>();
        }
    }
    if (unlikely(self->ctx.loadBL1Flag)) {
        LoadBL1Moudle<Intf>(self);
        self->ctx.bl1 = self->ctx.queueBL1.template DeQue<typename Intf::WeightT>();
    }

    LoadL0Moudle<Intf>(self, self->ctx.kL0FullLoadAl0PingPongFlag, self->ctx.kL0FullLoadBl0PingPongFlag, true);
}

template <class Intf, uint32_t ImplType>
__aicore__ bool Iterate<Intf, ImplType>::UpdateCommonIters(Intf *self, TempIters& tempIters)
{
    if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
        Intf::iterateMFirstFlag) {
        return UpdateCommonItersMModeMFirst<Intf>(self, tempIters);
    } else if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE) &&
        Intf::iterateNFirstFlag) {
        return UpdateCommonItersMModeNFirst<Intf>(self, tempIters);
    } else if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
        Intf::iterateMFirstFlag) {
        return UpdateCommonItersHWModeMFirst<Intf>(self, tempIters);
    } else if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE) &&
        Intf::iterateNFirstFlag) {
        return UpdateCommonItersHWModeNFirst<Intf>(self, tempIters);
    }
}

template <class Intf, uint32_t ImplType>
__aicore__ bool Iterate<Intf, ImplType>::UpdateItersByFmap(Intf *self, TempIters& tempIters, bool updateIterByFmapTag)
{
    if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
        tempIters.woAL1Iter = self->ctx.woAL1Iter;
        tempIters.hoAL1Iter = self->ctx.hoAL1Iter;
    } else {
        tempIters.mAL1Iter = self->ctx.mAL1Iter;
    }
    tempIters.batchIter = self->ctx.batchIter;
    self->ctx.kAL1Iter = self->ctx.kIter / self->ctx.multiKAL1;
    printf("kAL1Iter: %ld\n", self->ctx.kAL1Iter);
    tempIters.kAL1Iter = self->ctx.kAL1Iter + 1;
    if (tempIters.kAL1Iter <= self->ctx.maxKAL1Iter) {
        printf("kAL1Iter: %ld, maxKAL1Iter: %ld\n", tempIters.kAL1Iter, self->ctx.maxKAL1Iter);
        return false;
    }
    tempIters.kAL1Iter = 0;
    if (updateIterByFmapTag) {
        return UpdateCommonIters(self, tempIters);
    }
    if (tempIters.endTag) {
        return true;
    }
    return false;
}

template <class Intf, uint32_t ImplType>
__aicore__ bool Iterate<Intf, ImplType>::UpdateItersByWeight(Intf *self, TempIters& tempIters, bool updateIterByFmapTag)
{
    tempIters.nBL1Iter = self->ctx.nBL1Iter;
    self->ctx.kBL1Iter = self->ctx.kIter / self->ctx.multiKBL1;
    tempIters.kBL1Iter = self->ctx.kBL1Iter + 1;
    if (tempIters.kBL1Iter <= self->ctx.maxKBL1Iter) {
        return false;
    }
    tempIters.kBL1Iter = 0;
    if (!updateIterByFmapTag) {
        return UpdateCommonIters(self, tempIters);
    }
    if (tempIters.endTag) {
        return true;
    }
    return false;
}

template <class Intf, uint32_t ImplType>
__aicore__ void Iterate<Intf, ImplType>::ReduceKPreload(Intf *self)
{
    // state
    uint16_t al0PingPongFlag = 0;
    uint16_t bl0PingPongFlag = 0;
    bool isFirst = true;

    TempIters tempIters;
    // updateIterByFmapTag is true means fm update; false means weight update
    bool updateIterByFmapTag = self->ctx.convTiling->kAL1 > self->ctx.convTiling->kBL1;
    if (self->ctx.kAL1fullload && !self->ctx.kBL1fullload) {
        updateIterByFmapTag = false;
    } else if (!self->ctx.kAL1fullload && self->ctx.kBL1fullload) {
        updateIterByFmapTag = true;
    }
    if (self->ctx.loadAL1Flag || !self->ctx.kAL1fullload) {
        self->ctx.kAL1Iter = 0;
        // [zj]ToDo
        LoadAL1BaseMoudle<Intf>(self);
        if (self->ctx.kAL1fullload) {
            self->ctx.al1 = self->ctx.queueAL1.template DeQue<typename Intf::FmapT>();
            self->ctx.loadAL0Flag = true;
        }
    }

    if (self->ctx.loadBL1Flag || !self->ctx.kBL1fullload) {
        self->ctx.kBL1Iter = 0;
        LoadBL1BaseMoudle<Intf>(self);
        if (self->ctx.kBL1fullload) {
            self->ctx.bl1 = self->ctx.queueBL1.template DeQue<typename Intf::WeightT>();
            self->ctx.loadBL0Flag = true;
        }
    }

    while (self->ctx.kIter < self->ctx.ddr2l0LoopK) {
        if (self->ctx.loadAL1Flag || !self->ctx.kAL1fullload) {
            if (self->ctx.kIter % self->ctx.multiKAL1 == 0) {
                self->ctx.loadAl1Ins.SetSpr();
            }
            if (self->ctx.kIter % self->ctx.multiKAL1 == 0 && self->ctx.kIter < self->ctx.lastLoopKAL1StartPos) {
                if (self->ctx.kIter != 0) {
                    self->ctx.queueAL1.FreeTensor(self->ctx.al1);  // always freeTensor
                }
                if (!UpdateItersByFmap(self, tempIters, updateIterByFmapTag)) {
                    LoadAL1BaseMoudle<Intf>(self, tempIters);
                }
            }
            if (self->ctx.kIter == self->ctx.lastLoopKAL1StartPos) {
                self->ctx.queueAL1.FreeTensor(self->ctx.al1); // last iter of multiKAL1 only Free AL1 Tensor
            }
            if (self->ctx.kIter % self->ctx.multiKAL1 == 0) {
                self->ctx.al1 = self->ctx.queueAL1.template DeQue<typename Intf::FmapT>();
                self->ctx.loadAL0Flag = true;
            }
        }

        if (self->ctx.loadBL1Flag || !self->ctx.kBL1fullload) {
            if (self->ctx.kIter % self->ctx.multiKBL1 == 0 && self->ctx.kIter < self->ctx.lastLoopKBL1StartPos) {
                if (self->ctx.kIter != 0) {
                    self->ctx.queueBL1.FreeTensor(self->ctx.bl1); // always freeTensor
                }
                if (!UpdateItersByWeight(self, tempIters, updateIterByFmapTag)) {
                    LoadBL1BaseMoudle<Intf>(self, tempIters);
                }
            }
            if (self->ctx.kIter == self->ctx.lastLoopKBL1StartPos) {
                self->ctx.queueBL1.FreeTensor(self->ctx.bl1); // last iter of multiKBL1 only Free BL1 Tensor
            }
            if (self->ctx.kIter % self->ctx.multiKBL1 == 0) {
                self->ctx.bl1 = self->ctx.queueBL1.template DeQue<typename Intf::WeightT>();
                self->ctx.loadBL0Flag = true;
            }
        }

        LoadL0Moudle<Intf>(self, al0PingPongFlag, bl0PingPongFlag, isFirst);
        isFirst = false;
    }
}
 
template <class Intf, uint32_t ImplType>
__aicore__ void Iterate<Intf, ImplType>::ReduceK(Intf *self)
{
    // state
    uint16_t al0PingPongFlag = 0;
    uint16_t bl0PingPongFlag = 0;
    bool isFirst = true;

    while (self->ctx.kIter < self->ctx.ddr2l0LoopK) {
        if (self->ctx.loadAL1Flag || (!self->ctx.kAL1fullload && self->ctx.kIter % self->ctx.multiKAL1 == 0)) {
            if (self->ctx.kIter != 0) {
                self->ctx.queueAL1.FreeTensor(self->ctx.al1);
            }
            LoadAL1Moudle<Intf>(self);
            self->ctx.al1 = self->ctx.queueAL1.template DeQue<typename Intf::FmapT>();
            self->ctx.loadAL0Flag = true;
        }
        if (self->ctx.loadBL1Flag || (!self->ctx.kBL1fullload && self->ctx.kIter % self->ctx.multiKBL1 == 0)) {
            if (self->ctx.kIter != 0) {
                self->ctx.queueBL1.FreeTensor(self->ctx.bl1);
            }
            LoadBL1Moudle<Intf>(self);
            self->ctx.bl1 = self->ctx.queueBL1.template DeQue<typename Intf::WeightT>();
            self->ctx.loadBL0Flag = true;
        }

        LoadL0Moudle<Intf>(self, al0PingPongFlag, bl0PingPongFlag, isFirst);
        isFirst = false;
    }
}

template <class Intf, uint32_t ImplType>
__aicore__ void Iterate<Intf, ImplType>::IterateK(Intf *self)
{
    SetMNBeforeIterateK<Intf>(self);

    // [zj]ToDo
    if (self->ctx.enableBias) {
        if (!self->ctx.convTiling->biasFullLoadFlag) {
            self->ctx.biasL1 = self->ctx.queueBiasL1.template AllocTensor<typename Intf::BiasT>();
            self->ctx.loadBiasL1Ins.LoadChannelWiseL1(self->ctx.biasL1, self->ctx.biasgm);
            self->ctx.queueBiasL1.EnQue(self->ctx.biasL1);
            self->ctx.biasL1 = self->ctx.queueBiasL1.template DeQue<typename Intf::BiasT>();
        }
        self->ctx.biasBT = self->ctx.queueBiasBT.template AllocTensor<typename Intf::L0cT>();
        self->ctx.loadBiasBTIns.LoadBiasBt();
        self->ctx.queueBiasBT.EnQue(self->ctx.biasBT);
        self->ctx.biasBT = self->ctx.queueBiasBT.template DeQue<typename Intf::L0cT>();
    }
    // [zj]ToDo
    if (self->ctx.enableQuantPre) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                SetFlag<HardEvent::FIX_MTE2>(eventId);
                WaitFlag<HardEvent::FIX_MTE2>(eventId);
            }
            self->ctx.quantPreL1 = self->ctx.queueQuantPreL1.template AllocTensor<typename Intf::QuantPreT>();
            self->ctx.loadQuantPreL1Ins.LoadChannelWiseL1(self->ctx.quantPreL1, self->ctx.quantPreGm);
            self->ctx.queueQuantPreL1.EnQue(self->ctx.quantPreL1);
            self->ctx.quantPreL1 = self->ctx.queueQuantPreL1.template DeQue<typename Intf::QuantPreT>();
        }
#if defined(__DAV_L510__)
        self->ctx.quantPreFB = self->ctx.queueQuantPreFB.template AllocTensor<typename Intf::QuantPreT>();
        self->ctx.loadQuantPreFBIns.LoadFixpParamFb(DST_MEM_BLOCK_PRE_QUANT, self->ctx.quantPreFB,
                                                    self->ctx.quantPreL1);
        self->ctx.queueQuantPreFB.EnQue(self->ctx.quantPreFB);
        self->ctx.quantPreFB = self->ctx.queueQuantPreFB.template DeQue<typename Intf::QuantPreT>();
#endif
    }
    if (self->ctx.enableReluPre) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                SetFlag<HardEvent::FIX_MTE2>(eventId);
                WaitFlag<HardEvent::FIX_MTE2>(eventId);
            }
            self->ctx.reluPreL1 = self->ctx.queueReluPreL1.template AllocTensor<typename Intf::ReluPreT>();
            self->ctx.loadReluPreL1Ins.LoadChannelWiseL1(self->ctx.reluPreL1, self->ctx.reluPreGm);
            self->ctx.queueReluPreL1.EnQue(self->ctx.reluPreL1);
            self->ctx.reluPreL1 = self->ctx.queueReluPreL1.template DeQue<typename Intf::ReluPreT>();
        }
#if defined(__DAV_L510__)
        self->ctx.reluPreFB = self->ctx.queueReluPreFB.template AllocTensor<typename Intf::ReluPreT>();
        self->ctx.loadReluPreFBIns.LoadFixpParamFb(DST_MEM_BLOCK_PRE_RELU, self->ctx.reluPreFB, self->ctx.reluPreL1);
        self->ctx.queueReluPreFB.EnQue(self->ctx.reluPreFB);
        self->ctx.reluPreFB = self->ctx.queueReluPreFB.template DeQue<typename Intf::ReluPreT>();
#endif
    }
    if (self->ctx.enableEltAntiQuant) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                SetFlag<HardEvent::FIX_MTE2>(eventId);
                WaitFlag<HardEvent::FIX_MTE2>(eventId);
            }
            self->ctx.eltAntiQuantL1 =
                self->ctx.queueEltAntiQuantL1.template AllocTensor<typename Intf::EltAntiQuantT>();
            self->ctx.loadEltAntiQuantL1Ins.LoadChannelWiseL1(self->ctx.eltAntiQuantL1, self->ctx.eltAntiQuantGm);
            self->ctx.queueEltAntiQuantL1.EnQue(self->ctx.eltAntiQuantL1);
            self->ctx.eltAntiQuantL1 = self->ctx.queueEltAntiQuantL1.template DeQue<typename Intf::EltAntiQuantT>();
        }
#if defined(__DAV_L510__)
        self->ctx.eltAntiQuantFB = self->ctx.queueEltAntiQuantFB.template AllocTensor<typename Intf::EltAntiQuantT>();
        self->ctx.loadEltAntiQuantFBIns.LoadFixpParamFb(DST_MEM_BLOCK_ANTI_QUANT, self->ctx.eltAntiQuantFB,
                                                        self->ctx.eltAntiQuantL1);
        self->ctx.queueEltAntiQuantFB.EnQue(self->ctx.eltAntiQuantFB);
        self->ctx.eltAntiQuantFB = self->ctx.queueEltAntiQuantFB.template DeQue<typename Intf::EltAntiQuantT>();
#endif
    }
    if (self->ctx.enableQuantPost) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                SetFlag<HardEvent::FIX_MTE2>(eventId);
                WaitFlag<HardEvent::FIX_MTE2>(eventId);
            }
            self->ctx.quantPostL1 = self->ctx.queueQuantPostL1.template AllocTensor<typename Intf::QuantPostT>();
            self->ctx.loadQuantPostL1Ins.LoadChannelWiseL1(self->ctx.quantPostL1, self->ctx.quantPostGm);
            self->ctx.queueQuantPostL1.EnQue(self->ctx.quantPostL1);
            self->ctx.quantPostL1 = self->ctx.queueQuantPostL1.template DeQue<typename Intf::QuantPostT>();
        }
#if defined(__DAV_L510__)
        self->ctx.quantPostFB = self->ctx.queueQuantPostFB.template AllocTensor<typename Intf::QuantPostT>();
        self->ctx.loadQuantPostFBIns.LoadFixpParamFb(DST_MEM_BLOCK_POST_QUANT, self->ctx.quantPostFB,
                                                     self->ctx.quantPostL1);
        self->ctx.queueQuantPostFB.EnQue(self->ctx.quantPostFB);
        self->ctx.quantPostFB = self->ctx.queueQuantPostFB.template DeQue<typename Intf::QuantPostT>();
#endif
    }
    if (self->ctx.enableEltwise) {
        if (self->ctx.eltwiseBrcEn) {
            if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
                if constexpr (Intf::groupType != static_cast<int8_t>(ConvGroupType::NORMAL_CONV)) {
                    event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::FIX_MTE2));
                    SetFlag<HardEvent::FIX_MTE2>(eventId);
                    WaitFlag<HardEvent::FIX_MTE2>(eventId);
                }
                self->ctx.eltwiseL1 = self->ctx.queueEltwiseL1.template AllocTensor<typename Intf::EltwiseT>();
                self->ctx.loadEltwiseL1Ins.LoadChannelWiseL1(self->ctx.eltwiseL1, self->ctx.eltwiseGm);
                self->ctx.queueEltwiseL1.EnQue(self->ctx.eltwiseL1);
                self->ctx.eltwiseL1 = self->ctx.queueEltwiseL1.template DeQue<typename Intf::EltwiseT>();
            }
        } else {
            self->ctx.eltwiseL1 = self->ctx.queueEltwiseL1.template AllocTensor<typename Intf::EltwiseT>();
            self->ctx.loadEltwiseNonBrcL1Ins.CopyElt();
            self->ctx.queueEltwiseL1.EnQue(self->ctx.eltwiseL1);
            self->ctx.eltwiseL1 = self->ctx.queueEltwiseL1.template DeQue<typename Intf::EltwiseT>();
        }
    }

#if defined(__DAV_L510__)
    self->ctx.cl1 = self->ctx.queueCL1.template AllocTensor<typename Intf::OutputT>();
#else
    self->ctx.cl0 = self->ctx.queueCL0.template AllocTensor<typename Intf::L0cT>();
#endif

    // reduceK priority: 1.KL0FullLoad 2.L1DB Preload 3. ordinary reduceK
    if constexpr (Intf::kl0FullLoadFlag) {
        ReduceOneK(self);
    } else if constexpr (Intf::kPreLoadFlag) {
        ReduceKPreload(self);
    } else {
        ReduceK(self);
    }

    FreeL1Tensor<Intf>(self);

#if defined(__DAV_L510__)
    self->ctx.queueCL1.EnQue(self->ctx.cl1);
    self->ctx.cl1 = self->ctx.queueCL1.template DeQue<typename Intf::OutputT>();
#else
    self->ctx.queueCL0.EnQue(self->ctx.cl0);
    self->ctx.cl0 = self->ctx.queueCL0.template DeQue<typename Intf::L0cT>();
#endif

    self->ctx.kIter = 0;
}

template <class Intf, uint32_t ImplType>
__aicore__ bool Iterate<Intf, ImplType>::IterateImplHWMode(Intf *self, bool enPartialSum)
{
    if (self->ctx.isFirstIterate) {
        FirstIterateImplHWMode<Intf>(self);
    } else if constexpr (Intf::iterateMFirstFlag) {
        if (IterateMFirstHWMode<Intf>(self) == false) {
            return false;
        }
    } else if constexpr (Intf::iterateNFirstFlag) {
        if (IterateNFirstHWMode<Intf>(self) == false) {
            return false;
        }
    }

    UpdateHoL0WoL0<Intf>(self);

    IterateK(self);

    return true;
}

template <class Intf, uint32_t ImplType>
__aicore__ bool Iterate<Intf, ImplType>::IterateImplMMode(Intf *self, bool enPartialSum)
{
    if (self->ctx.isFirstIterate) {
        FirstIterateImplMMode(self);
        // [zj]ToDo
        if (Intf::mPreLoadAFlag) {
            printf("first iterate fmap preload\n");
            self->ctx.kAL1Iter = self->ctx.kIter / self->ctx.multiKAL1;
            LoadAL1BaseMoudle<Intf>(self);
            self->ctx.loadAL1Flag = true;
        }
    } else if constexpr (Intf::iterateMFirstFlag) {
        if (IterateMFirstMMode(self) == false) {
            return false;
        }
    } else if constexpr (Intf::iterateNFirstFlag) {
        if (IterateNFirstMMode(self) == false) {
            return false;
        }
    }

    IterateK(self);

    return true;
}

};


#endif // CONV_ITERATE_IMPL_H