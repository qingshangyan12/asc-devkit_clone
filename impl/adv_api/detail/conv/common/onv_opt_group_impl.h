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
 * \file conv_opt_group_impl.h
 * \brief
 */

#ifndef CONV_OPT_GROUP_IMPL_H
#define CONV_OPT_GROUP_IMPL_H

#include "conv_config.h"
#include "conv_framework_util.h"
#include "conv_opt_group_init_impl.h"
#include "conv_util.h"

namespace ConvFunc {
using namespace AscendC;
using namespace conv;

CONV_DECLARE_REG_IMPL(SetWeightStartPosition);
CONV_DECLARE_REG_IMPL(SetIterIndex);

template <class Intf, uint32_t ImplType>
struct SetWeightStartPosition {
    static __aicore__ inline void call(Intf *self, int64_t coStartPos, int64_t ciStartPos = 0)
    {
        self->ctx.coStartPos = coStartPos;
        self->ctx.ciStartPos = ciStartPos;
    }
};

template <class Intf, uint32_t ImplType>
struct SetIterIndex {
    static __aicore__ inline void call(Intf *self, uint64_t groupOptIter)
    {
        self->ctx.groupOptIter = groupOptIter;
        if ASCEND_IS_AIC {
            self->ctx.vecId = (groupOptIter % VEC_NUM) * VEC_ID_MAX;
        }
    }
};

template <class Intf>
__aicore__ inline bool OptGroupUpdateBL1(Intf *self)
{
    if (!((self->ctx.kIter == self->ctx.ddr2l0LoopK - 1) ||
          ((self->ctx.kIter + 1) % self->ctx.multiKBL1 == 0))) {
        return false;
    }

    // In !kBL1fullload scene, bL1 update in kReduce.
    if (!self->ctx.kBL1fullload) {
        return true;
    }

    // In kBL1fullload scene, bL1 update when nBL1 update.
    bool updateBL1Flag = false;
    if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
        if constexpr (Intf::iterateMFirstFlag) {
            updateBL1Flag = self->ctx.woL0Iter == self->ctx.maxWoL0Iter &&
                            self->ctx.hoL0Iter == self->ctx.maxHoL0Iter &&
                            self->ctx.nL0Iter == self->ctx.maxNL0Iter &&
                            self->ctx.woAL1Iter == self->ctx.maxWoL1Iter &&
                            self->ctx.hoAL1Iter == self->ctx.maxHoL1Iter &&
                            self->ctx.batchIter == self->ctx.ddr2l1LoopBatch - 1;
        } else {
            updateBL1Flag = self->ctx.nL0Iter == self->ctx.maxNL0Iter &&
                            self->ctx.woL0Iter == self->ctx.maxWoL0Iter &&
                            self->ctx.hoL0Iter == self->ctx.maxHoL0Iter;
        }
    } else if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
        if constexpr (Intf::iterateMFirstFlag) {
            updateBL1Flag = self->ctx.mL0Iter == self->ctx.maxML0Iter &&
                            self->ctx.nL0Iter == self->ctx.maxNL0Iter &&
                            self->ctx.mAL1Iter == self->ctx.maxMAL1Iter &&
                            self->ctx.batchIter == self->ctx.ddr2l1LoopBatch - 1;
        } else {
            updateBL1Flag = self->ctx.nL0Iter == self->ctx.maxNL0Iter &&
                            self->ctx.mL0Iter == self->ctx.maxML0Iter;
        }
    }

    if constexpr (Intf::iterateMFirstFlag && Intf::formatC == ConvFormat::NCDHW) {
        updateBL1Flag = updateBL1Flag && self->ctx.dOutIter == self->ctx.ddr2l1LoopD - 1;
    }

    return updateBL1Flag;
}

template <class Intf>
__aicore__ inline void OptGroupSyncSet(Intf *self)
{
    if (!OptGroupUpdateBL1<Intf>(self)) {
        return;
    }

    if constexpr (Intf::bL1DBFlag) {
        if (self->ctx.loadUB2L1Iter < self->ctx.bL1LoadTimes - 1) {
            CrossCoreSetFlag<CV_ENHANCE_MODE, PIPE_MTE1>(self->ctx.vecId + CV_SYNC_ID_MTE1_MTE3);
            return;
        } else if (self->ctx.loadUB2L1Iter == self->ctx.bL1LoadTimes - 1) {
            return;
        }
    } else {
        if (self->ctx.loadUB2L1Iter < self->ctx.bL1LoadTimes) {
            CrossCoreSetFlag<CV_ENHANCE_MODE, PIPE_MTE1>(self->ctx.vecId + CV_SYNC_ID_MTE1_MTE3);
            return;
        }
    }

    if ((self->ctx.groupOptIter != self->ctx.singleGroupOpt - 1)) {
        // Vec0 and Vec1 alternate load groupOpt, such as vec0-groupOpt0 -> vec1-groupOpt1 -> vec0-groupOpt2...
        // Notify next vec start mov ub to l1
        CrossCoreSetFlag<CV_ENHANCE_MODE, PIPE_MTE1>((self->ctx.vecId ^ VEC_ID_MAX) + CV_SYUC_ID_V_ROTATE);
    }
}

template <class Intf>
__aicore__ inline void OptGroupUpdateLoopN(Intf *self) {
    if constexpr (Intf::hasNL0IterFlag) {
        self->ctx.l12l0LoopN = self->ctx.nBL1Iter == self->ctx.maxNBL1Iter ?
            CeilDIV(self->ctx.nBL1Tail, self->ctx.convTiling->nL0) : self->ctx.convTiling->multiNBL1;
        self->ctx.ddr2l1LoopInner = self->ctx.ddr2l1LoopTmp * self->ctx.l12l0LoopN;
    }
}

template <class Intf>
__aicore__ inline void OptGroupUpdateLoopInner(Intf *self) {
    if (!self->ctx.kBL1fullload) {
        if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            self->ctx.ddr2l1LoopTmp = self->ctx.outerIter == self->ctx.maxMAL1Iter ?
                CeilDIV(self->ctx.mAL1Tail, self->ctx.mL0) : self->ctx.l12l0LoopM;
        } else {
            if constexpr (Intf::hasWL1IterFlag) {
                self->ctx.woAL1Iter = self->ctx.outerIter % self->ctx.ddr2l1LoopW;
                if (self->ctx.woL1SmallTail > 0) {
                    if (self->ctx.woAL1Iter == self->ctx.maxWoL1Iter) {
                        self->ctx.currentWoL1 = self->ctx.woL1SmallTail;
                    } else if (self->ctx.woAL1Iter == self->ctx.maxWoL1Iter - 1) {
                        self->ctx.currentWoL1 = self->ctx.woAL1Tail;
                    } else {
                        self->ctx.currentWoL1 = self->ctx.convTiling->woL1;
                    }
                } else {
                    self->ctx.currentWoL1 = self->ctx.woAL1Iter == self->ctx.maxWoL1Iter ?
                        self->ctx.woAL1Tail : self->ctx.convTiling->woL1;
                }
            }
            if constexpr (Intf::hasWL0IterFlag) {
                self->ctx.l12l0LoopW = CeilDIV(self->ctx.currentWoL1, self->ctx.convTiling->woL0);
            }

            if constexpr (Intf::hasHL1IterFlag) {
                self->ctx.hoAL1Iter = (self->ctx.outerIter / self->ctx.ddr2l1LoopW) % self->ctx.ddr2l1LoopH;
                self->ctx.currentHoL1 = self->ctx.hoAL1Iter == self->ctx.maxHoL1Iter ?
                    self->ctx.hoAL1Tail : self->ctx.convTiling->hoL1;
            }
            if constexpr (Intf::hasHL0IterFlag) {
                self->ctx.l12l0LoopH = CeilDIV(self->ctx.currentHoL1, self->ctx.convTiling->hoL0);
            }

            self->ctx.ddr2l1LoopTmp = self->ctx.l12l0LoopW * self->ctx.l12l0LoopH;
        }
        self->ctx.ddr2l1LoopInner = self->ctx.ddr2l1LoopTmp * self->ctx.l12l0LoopN;
    }
}

template <class Intf>
__aicore__ inline bool OptGroupUB2L1Iter(Intf *self)
{
    if (self->ctx.loadUB2L1Iter == 0) {
        return true;
    }

    if (!self->ctx.kBL1fullload) {
        self->ctx.kBL1Iter++;
        if (self->ctx.kBL1Iter == self->ctx.ddr2l1LoopKB) {
            self->ctx.kBL1Iter = 0;
        } else {
            return true;
        }

        // Total iter merge before nBL1Iter.
        self->ctx.innerIter++;
        if (self->ctx.innerIter == self->ctx.ddr2l1LoopInner) {
            self->ctx.innerIter = 0;
        } else {
            return true;
        }
    }

    if constexpr (Intf::hasNL1IterFlag) {
        self->ctx.nBL1Iter++;
        if (self->ctx.nBL1Iter == self->ctx.ddr2l1LoopN) {
            self->ctx.nBL1Iter = 0;
            OptGroupUpdateLoopN<Intf>(self);
        } else {
            OptGroupUpdateLoopN<Intf>(self);
            return true;
        }
    }

    if constexpr (Intf::iterateNFirstFlag) {
        // Total iter merge after nBL1Iter.
        self->ctx.outerIter++;
        if (self->ctx.outerIter != self->ctx.ddr2l1LoopOuter) {
            return true;
        }
    }

    return false;
}

template <class Intf>
__aicore__ inline void OptGroupIterInit(Intf *self)
{
    self->ctx.kBL1Iter = 0;
    self->ctx.nBL1Iter = 0;
    self->ctx.innerIter = 0;
    self->ctx.outerIter = 0;
    self->ctx.loadUB2L1Iter = 0;
    self->ctx.pingPongFlag = self->ctx.vecId == 0 ? 0 : self->ctx.bL1LoadTimes % DOUBLE_BUF;
    OptGroupUpdateLoopN<Intf>(self);
    if constexpr (Intf::iterateNFirstFlag) {
        OptGroupUpdateLoopInner<Intf>(self);
    }
    self->ctx.ddr2l1LoopInner = self->ctx.ddr2l1LoopTmp * self->ctx.l12l0LoopN;
}

template <class Intf>
__aicore__ inline bool OptGroupVecImpl(Intf *self)
{
    if (self->ctx.vecId != (self->ctx.groupOptIter % VEC_NUM)) {
        return false;
    }

    // For next nddma wait nd2nz
    event_t eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::V_MTE2));
    SetFlag<HardEvent::V_MTE2>(eventId);
    WaitFlag<HardEvent::V_MTE2>(eventId);

    self->ctx.loadGm2UBTools.LoadGM2UB();

    // For nd2nz wait nddma
    eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::MTE2_V));
    SetFlag<HardEvent::MTE2_V>(eventId);
    WaitFlag<HardEvent::MTE2_V>(eventId);
    // For next nd2nz wait ub2l1
    eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::MTE3_V));
    SetFlag<HardEvent::MTE3_V>(eventId);
    WaitFlag<HardEvent::MTE3_V>(eventId);

    self->ctx.transND2NZTools.TransND2NZ();

    // For ub2l1 wait nd2nz
    eventId = static_cast<event_t>(self->ctx.pipe->FetchEventID(HardEvent::V_MTE3));
    SetFlag<HardEvent::V_MTE3>(eventId);
    WaitFlag<HardEvent::V_MTE3>(eventId);

    OptGroupIterInit<Intf>(self);
    if (self->ctx.groupOptIter != 0) {
        // CrossCoreWaitFlag<CV_ENHANCE_MODE, PIPE_MTE3>(CV_SYUC_ID_V_ROTATE);
    }
    while (OptGroupUB2L1Iter<Intf>(self)) {
        if constexpr (Intf::bL1DBFlag) {
            if (self->ctx.loadUB2L1Iter > 1) {
                // CrossCoreWaitFlag<CV_ENHANCE_MODE, PIPE_MTE3>(CV_SYNC_ID_MTE1_MTE3);
            }
        } else {
            if (self->ctx.loadUB2L1Iter > 0) {
                // CrossCoreWaitFlag<CV_ENHANCE_MODE, PIPE_MTE3>(CV_SYNC_ID_MTE1_MTE3);
            }
        }
        self->ctx.loadUB2L1Tools.LoadUB2L1();
        CrossCoreSetFlag<CV_ENHANCE_MODE, PIPE_MTE3>(CV_SYNC_ID_MTE3_MTE1);

        self->ctx.loadUB2L1Iter++;
    }

    return false;
}

};

#endif // CONV_OPT_GROUP_IMPL_H
