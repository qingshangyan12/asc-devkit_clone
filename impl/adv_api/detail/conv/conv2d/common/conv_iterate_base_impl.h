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
 * \file conv_iterate_base_impl.h
 * \brief
 */

#ifndef CONV_ITERATE_BASE_IMPL_H
#define CONV_ITERATE_BASE_IMPL_H

#include "conv_config.h"
#include "conv_opt_group_impl.h"
#include "conv_util.h"

namespace ConvFunc {
using namespace AscendC;
using namespace conv;

struct TempIters {
    uint64_t kAL1Iter = 0;
    uint64_t kBL1Iter = 0;
    uint64_t mAL1Iter = 0; // m mode
    uint64_t hoAL1Iter = 0; // hw mode
    uint64_t woAL1Iter = 0; // hw mode
    uint64_t nBL1Iter = 0;
    uint64_t batchIter = 0;
    bool endTag = false;
};

template <class Intf>
static __aicore__ inline void InitCoDirectionValue(Intf *self)
{
    self->ctx.nBL1Tail = self->ctx.singleCoreCo % self->ctx.convTiling->nBL1;
    self->ctx.nBL1Tail = self->ctx.nBL1Tail == 0 ? self->ctx.convTiling->nBL1 : self->ctx.nBL1Tail;
    self->ctx.nL0Tail = self->ctx.nBL1Tail % self->ctx.convTiling->nL0;
    self->ctx.nL0Tail = self->ctx.nL0Tail == 0 ? self->ctx.convTiling->nL0 : self->ctx.nL0Tail;

    if constexpr (!Intf::hasNL1IterFlag) {
        self->ctx.maxNBL1Iter = 0;
        self->ctx.ddr2l1LoopN = 1;
    } else {
        self->ctx.ddr2l1LoopN = CeilDIV(self->ctx.singleCoreCo, self->ctx.convTiling->nBL1);
        self->ctx.maxNBL1Iter = self->ctx.ddr2l1LoopN - 1;
    }

    if constexpr (!Intf::hasNL0IterFlag) {
        self->ctx.l12l0LoopN = 1;
        self->ctx.maxNL0Iter = 0;
    } else if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        self->ctx.l12l0LoopN = self->ctx.nBL1Iter == self->ctx.maxNBL1Iter ?
            CeilDIV(self->ctx.nBL1Tail, self->ctx.convTiling->nL0) : self->ctx.convTiling->multiNBL1;
    }

    self->ctx.currentNBL1 = self->ctx.nBL1Tail;
}

template <class Intf>
__aicore__ inline void CalcCoDirectionVar(Intf *self)
{
    if constexpr (Intf::hasNL0IterFlag) {
        self->ctx.l12l0LoopN = self->ctx.nBL1Iter == self->ctx.maxNBL1Iter ?
            CeilDIV(self->ctx.nBL1Tail, self->ctx.convTiling->nL0) : self->ctx.convTiling->multiNBL1;
        self->ctx.maxNL0Iter = self->ctx.l12l0LoopN - 1;
    }
}

template <class Intf>
__aicore__ inline uint64_t CalcCurrentNL0(Intf *self)
{
    bool isNL0Tail = self->ctx.nBL1Iter == self->ctx.maxNBL1Iter && self->ctx.nL0Iter == self->ctx.maxNL0Iter;
    uint64_t currentNL0 = isNL0Tail ? self->ctx.nL0Tail : self->ctx.convTiling->nL0;
    self->ctx.currentNL0Align = isNL0Tail ? AlignB(currentNL0, BLOCK_L0_N) : currentNL0;

    return currentNL0;
}

template <class Intf>
__aicore__ inline uint64_t CalcCurrentML0MMode(Intf *self)
{
    printf("mL0: %ld, mAL0Tail: %ld, mAL1Iter: %ld, maxMAL1Iter: %ld, mL0Iter: %ld, maxML0Iter: %ld\n", self->ctx.mL0,
           self->ctx.mAL0Tail, self->ctx.mAL1Iter, self->ctx.maxMAL1Iter, self->ctx.mL0Iter, self->ctx.maxML0Iter);
    bool isML0Tail = self->ctx.mAL1Iter == self->ctx.maxMAL1Iter && self->ctx.mL0Iter == self->ctx.maxML0Iter;
    uint64_t currentML0 = isML0Tail ? self->ctx.mAL0Tail : self->ctx.mL0;
    self->ctx.currentML0Align = isML0Tail ? AlignB(currentML0, BLOCK_L0_M) : currentML0;

    return currentML0;
}

template <class Intf>
__aicore__ inline uint64_t CalcCurrentML0HWMode(Intf *self)
{
    uint64_t currentML0 = self->ctx.currentHoL0 * self->ctx.currentWoL0;
    if (self->ctx.currentWoL0 == self->ctx.convTiling->woL0 &&
        self->ctx.currentHoL0 == self->ctx.convTiling->hoL0) {
        self->ctx.currentML0Align = self->ctx.convTiling->mStep;
    } else {
        self->ctx.currentML0Align = AlignB(currentML0, BLOCK_L0_N);
    }

    return currentML0;
}

template <class Intf>
__aicore__ inline void LoadAL1BaseMoudle(Intf *self)
{
    self->ctx.al1 = self->ctx.queueAL1.template AllocTensor<typename Intf::FmapT>();
    self->ctx.loadAl1Ins.LoadAL1();
    self->ctx.queueAL1.EnQue(self->ctx.al1);
    self->ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK
}

template <class Intf>
__aicore__ inline void LoadAL1BaseMoudle(Intf *self, TempIters& tempIters)
{
    self->ctx.al1 = self->ctx.queueAL1.template AllocTensor<typename Intf::FmapT>();
    if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        self->ctx.loadAl1Ins.LoadAL1(tempIters.kAL1Iter, tempIters.mAL1Iter, tempIters.batchIter);
    } else {
        self->ctx.loadAl1Ins.LoadAL1(tempIters.kAL1Iter, tempIters.woAL1Iter, tempIters.hoAL1Iter, tempIters.batchIter);
    }
    self->ctx.queueAL1.EnQue(self->ctx.al1);
    self->ctx.loadAL1Flag = false;  // Only k directrion can be reloaded in LoopK
}

template <class Intf>
__aicore__ inline void LoadBL1BaseMoudle(Intf *self)
{
    self->ctx.bl1 = self->ctx.queueBL1.template AllocTensor<typename Intf::WeightT>();
    if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        // CrossCoreWaitFlag<CV_ENHANCE_MODE, PIPE_MTE1>(self->ctx.vecId + CV_SYNC_ID_MTE3_MTE1);
        self->ctx.loadUB2L1Iter++;
    } else {
        self->ctx.loadBL1Ins.LoadBL1();
    }
    self->ctx.queueBL1.EnQue(self->ctx.bl1);
    self->ctx.loadBL1Flag = false;  // Only k directrion can be reloaded in LoopK
}

template <class Intf>
__aicore__ inline void LoadBL1BaseMoudle(Intf *self, TempIters& tempIters)
{
    self->ctx.bl1 = self->ctx.queueBL1.template AllocTensor<typename Intf::WeightT>();
    self->ctx.loadBL1Ins.LoadBL1(tempIters.kBL1Iter, tempIters.nBL1Iter);
    self->ctx.queueBL1.EnQue(self->ctx.bl1);
    self->ctx.loadBL1Flag = false;  // Only k directrion can be reloaded in LoopK
}

template <class Intf>
__aicore__ inline void LoadAL1Moudle(Intf *self)
{
    self->ctx.kAL1Iter = self->ctx.kIter / self->ctx.multiKAL1;
    LoadAL1BaseMoudle<Intf>(self);
}

template <class Intf>
__aicore__ inline void LoadBL1Moudle(Intf *self)
{
    self->ctx.kBL1Iter = self->ctx.kIter / self->ctx.multiKBL1;
    LoadBL1BaseMoudle<Intf>(self);
}

template <class Intf>
__aicore__ inline void ReduceKCloseL0PingPong(Intf *self, bool isFirst)
{
    // [zj]ToDo

    printf("isFirst: %d, iter: %d\n", isFirst, self->ctx.iter);
    if (self->ctx.iter >= 1) {
        wait_flag(PIPE_M, PIPE_MTE1, 0x0);
    }
    if (!((self->ctx.convTiling->kAL1 == self->ctx.convTiling->kL0) && (!self->ctx.loadAL0Flag))) {
        self->ctx.al0 = self->ctx.wholeAl0Tensor;
        self->ctx.kAL0Iter = self->ctx.kIter % self->ctx.multiKAL1;
        self->ctx.loadAL0Ins.LoadAL0(isFirst);
    }
    if (!((self->ctx.convTiling->kBL1 == self->ctx.convTiling->kL0) && (!self->ctx.loadBL0Flag))) {
        self->ctx.bl0 = self->ctx.wholeBl0Tensor;
        self->ctx.kBL0Iter = self->ctx.kIter % self->ctx.multiKBL1;
        self->ctx.loadBL0Ins.LoadBL0(isFirst);
        if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            OptGroupSyncSet<Intf>(self);
        }
    }
    set_flag(PIPE_MTE1, PIPE_M, 0x0);
    wait_flag(PIPE_MTE1, PIPE_M, 0x0);
    self->ctx.madIns.Mad();
    set_flag(PIPE_M, PIPE_MTE1, 0x0);
    self->ctx.kIter++;
}

template <class Intf>
__aicore__ inline void ReduceKOpenL0APingPong(Intf *self, uint16_t& al0PingPongFlag, bool isFirst)
{
    // [zj]ToDo

    if (self->ctx.iter >= DOUBLE_BUF) {
        wait_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
    }
    self->ctx.al0 =
        self->ctx.wholeAl0Tensor[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / Intf::sizeOfFmap];
    self->ctx.kAL0Iter = self->ctx.kIter % self->ctx.multiKAL1;
    self->ctx.loadAL0Ins.LoadAL0(isFirst);

    if (!((self->ctx.convTiling->kBL1 == self->ctx.convTiling->kL0) && (!self->ctx.loadBL0Flag))) {
        self->ctx.bl0 = self->ctx.wholeBl0Tensor;
        self->ctx.kBL0Iter = self->ctx.kIter % self->ctx.multiKBL1;
        self->ctx.loadBL0Ins.LoadBL0(isFirst);
        if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
            OptGroupSyncSet<Intf>(self);
        }
    }
    set_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
    wait_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
    self->ctx.madIns.Mad();
    set_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
    al0PingPongFlag += 1;
    self->ctx.kIter++;
}

template <class Intf>
__aicore__ inline void ReduceKOpenL0BPingPong(Intf *self, uint16_t& bl0PingPongFlag, bool isFirst)
{
    // [zj]ToDo

    printf("bl0PingPongFlag: %u, isFirst: %d, iter: %d\n", bl0PingPongFlag, isFirst, self->ctx.iter);
    if (self->ctx.iter >= DOUBLE_BUF) {
        wait_flag(PIPE_M, PIPE_MTE1, (bl0PingPongFlag & 0x1));
    }
    if (!((self->ctx.convTiling->kAL1 == self->ctx.convTiling->kL0) && (!self->ctx.loadAL0Flag))) {
        self->ctx.al0 = self->ctx.wholeAl0Tensor;
        self->ctx.kAL0Iter = self->ctx.kIter % self->ctx.multiKAL1;
        self->ctx.loadAL0Ins.LoadAL0(isFirst);
    }

    self->ctx.bl0 =
        self->ctx.wholeBl0Tensor[(bl0PingPongFlag & 0x1) * L0B_HALF_SIZE / Intf::sizeOfWeight];
    self->ctx.kBL0Iter = self->ctx.kIter % self->ctx.multiKBL1;
    self->ctx.loadBL0Ins.LoadBL0(isFirst);
    if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        OptGroupSyncSet<Intf>(self);
    }
    set_flag(PIPE_MTE1, PIPE_M, bl0PingPongFlag & 0x1);
    wait_flag(PIPE_MTE1, PIPE_M, bl0PingPongFlag & 0x1);
    self->ctx.madIns.Mad();
    set_flag(PIPE_M, PIPE_MTE1, bl0PingPongFlag & 0x1);
    bl0PingPongFlag += 1;
    self->ctx.kIter++;
}

template <class Intf>
__aicore__ inline void ReduceKOpenL0AL0BPingPong(Intf *self, uint16_t& al0PingPongFlag, bool isFirst)
{
    self->ctx.al0 =
        self->ctx.wholeAl0Tensor[(al0PingPongFlag & 0x1) * L0A_HALF_SIZE / Intf::sizeOfFmap];
    self->ctx.bl0 =
        self->ctx.wholeBl0Tensor[(al0PingPongFlag & 0x1) * L0B_HALF_SIZE / Intf::sizeOfWeight];
    // [zj]ToDo

    if (self->ctx.iter >= DOUBLE_BUF) {
        wait_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
    }
    self->ctx.kAL0Iter = self->ctx.kIter % self->ctx.multiKAL1;
    self->ctx.loadAL0Ins.LoadAL0(isFirst);
    self->ctx.kBL0Iter = self->ctx.kIter % self->ctx.multiKBL1;
    self->ctx.loadBL0Ins.LoadBL0(isFirst);
    if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
        OptGroupSyncSet<Intf>(self);
    }
    set_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
    wait_flag(PIPE_MTE1, PIPE_M, al0PingPongFlag & 0x1);
    self->ctx.madIns.Mad();
    set_flag(PIPE_M, PIPE_MTE1, al0PingPongFlag & 0x1);
    al0PingPongFlag += 1;
    self->ctx.kIter++;
}

template <class Intf>
__aicore__ inline void LoadL0Moudle(Intf *self, uint16_t &al0PingPongFlag, uint16_t &bl0PingPongFlag, bool isFirst)
{
    if constexpr (Intf::l0PingPong == static_cast<int32_t>(ConvL0PingPong::ALL_CLOSE)) {
        ReduceKCloseL0PingPong<Intf>(self, isFirst);
    } else if constexpr (Intf::l0PingPong == static_cast<int32_t>(ConvL0PingPong::L0A_OPEN)) {
        ReduceKOpenL0APingPong<Intf>(self, al0PingPongFlag, isFirst);
    } else if constexpr (Intf::l0PingPong == static_cast<int32_t>(ConvL0PingPong::L0B_OPEN)) {
        ReduceKOpenL0BPingPong<Intf>(self, bl0PingPongFlag, isFirst);
    } else if constexpr (Intf::l0PingPong == static_cast<int32_t>(ConvL0PingPong::ALL_OPEN)) {
        ReduceKOpenL0AL0BPingPong<Intf>(self, al0PingPongFlag, isFirst);
    }
    self->ctx.iter++;
}

template <class Intf>
__aicore__ inline void SetMNBeforeIterateK(Intf *self)
{
    uint64_t currentML0 = 0;
    if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        currentML0 = CalcCurrentML0MMode<Intf>(self);
    } else {
        currentML0 = CalcCurrentML0HWMode<Intf>(self);
    }
    printf("currentML0: %ld\n", currentML0);
    uint64_t currentNL0 = CalcCurrentNL0<Intf>(self);
    self->ctx.loadAL0Ins.SetM(self->ctx.currentML0Align, currentML0);
    self->ctx.loadBL0Ins.SetN(self->ctx.currentNL0Align);
    self->ctx.loadBiasL1Ins.SetN(currentNL0);
    self->ctx.loadQuantPreL1Ins.SetN(currentNL0);
    self->ctx.loadReluPreL1Ins.SetN(currentNL0);
    self->ctx.loadEltAntiQuantL1Ins.SetN(currentNL0);
    self->ctx.loadQuantPostL1Ins.SetN(currentNL0);
    self->ctx.loadEltwiseL1Ins.SetN(currentNL0);
    self->ctx.loadEltwiseNonBrcL1Ins.SetMN(currentML0, self->ctx.currentNL0Align);
    self->ctx.loadBiasBTIns.SetN(self->ctx.currentNL0Align);
    self->ctx.loadQuantPreFBIns.SetN(self->ctx.currentNL0Align);
    self->ctx.loadReluPreFBIns.SetN(self->ctx.currentNL0Align);
    self->ctx.loadEltAntiQuantFBIns.SetN(self->ctx.currentNL0Align);
    self->ctx.loadQuantPostFBIns.SetN(self->ctx.currentNL0Align);
    self->ctx.madIns.SetMN(self->ctx.currentML0Align, currentML0, self->ctx.currentNL0Align);
    if constexpr (Intf::formatC == ConvFormat::NC1HWC0) {
        self->ctx.copyOutIns.SetMN(currentML0, self->ctx.currentNL0Align);
    } else {
        self->ctx.copyOutIns.SetMN(currentML0, currentNL0);
    }
}

template <class Intf>
__aicore__ inline void FreeL1Tensor(Intf *self)
{
    if (!self->ctx.kAL1fullload) {
        self->ctx.queueAL1.FreeTensor(self->ctx.al1);
    }
    if (!self->ctx.kBL1fullload) {
        self->ctx.queueBL1.FreeTensor(self->ctx.bl1);
    }
    if (self->ctx.enableBias) {
        if (!self->ctx.convTiling->biasFullLoadFlag) {
            self->ctx.queueBiasL1.FreeTensor(self->ctx.biasL1);
        }
        self->ctx.queueBiasBT.FreeTensor(self->ctx.biasBT);
    }
    // [zj]
    if (self->ctx.enableQuantPre) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            self->ctx.queueQuantPreL1.FreeTensor(self->ctx.quantPreL1);
        }
        self->ctx.queueQuantPreFB.FreeTensor(self->ctx.quantPreFB);
    }
    if (self->ctx.enableReluPre) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            self->ctx.queueReluPreL1.FreeTensor(self->ctx.reluPreL1);
        }
        self->ctx.queueReluPreFB.FreeTensor(self->ctx.reluPreFB);
    }
    if (self->ctx.enableEltAntiQuant) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            self->ctx.queueEltAntiQuantL1.FreeTensor(self->ctx.eltAntiQuantL1);
        }
        self->ctx.queueEltAntiQuantFB.FreeTensor(self->ctx.eltAntiQuantFB);
    }
    if (self->ctx.enableQuantPost) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            self->ctx.queueQuantPostL1.FreeTensor(self->ctx.quantPostL1);
        }
        self->ctx.queueQuantPostFB.FreeTensor(self->ctx.quantPostFB);
    }
    if (self->ctx.enableEltwise && self->ctx.eltwiseBrcEn) {
        if (!self->ctx.convTiling->fixpParamsFullLoadFlag) {
            self->ctx.queueEltwiseL1.FreeTensor(self->ctx.eltwiseL1);
        }
    }
}
};

#endif // CONV_ITERATE_BASE_IMPL_H