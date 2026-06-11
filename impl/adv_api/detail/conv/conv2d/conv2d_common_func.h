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
 * \file conv2d_common_func.h
 * \brief
 */

#ifndef CONV2D_COMMON_FUNC_H
#define CONV2D_COMMON_FUNC_H

#include "../conv_common/conv_common_func.h"
#include "../conv_common/conv_framework_util.h"
#include "../conv_common/impl/conv_instr.h"
#include "lib/conv/conv2d_config.h"
#include "conv2d_util.h"
#include "impl/conv2d_instr.h"

namespace Conv2dFunc {
using namespace ConvFunc;
using namespace conv;

CONV_DECLARE_REG_IMPL(Init);
CONV_DECLARE_REG_IMPL(SetOrgFmapShape);
CONV_DECLARE_REG_IMPL(SetOrgWeightShape);
CONV_DECLARE_REG_IMPL(SetOrgOutputShape);
CONV_DECLARE_REG_IMPL(SetSingleFmapShape);
CONV_DECLARE_REG_IMPL(SetSingleOutputShape);
CONV_DECLARE_REG_IMPL(SetFmapStartPosition);
CONV_DECLARE_REG_IMPL(SetOptGroupParams);

using TypeFalse = struct {
    __uint128_t _[1024];
};

template <class Intf>
static __aicore__ inline void InitKDirectionValue(Intf *self)
{
    // K
    if constexpr(Intf::formatB == ConvFormat::FRACTAL_Z_C04) {
        size_t multiKL1 = self->ctx.convTiling->kAL1 / self->ctx.convTiling->kL0;
        self->ctx.maxKAL1Iter = 0;
        self->ctx.maxKBL1Iter = 0;
        self->ctx.maxKL0Iter = multiKL1 - 1;
        self->ctx.ddr2l0LoopK = multiKL1;
        self->ctx.kAL1Tail = self->ctx.convTiling->kAL1;
        self->ctx.kBL1Tail = self->ctx.convTiling->kBL1;
        self->ctx.kL0Tail = self->ctx.convTiling->kL0;
        self->ctx.multiKAL1 = multiKL1;
        self->ctx.multiKBL1 = multiKL1;
        self->ctx.kAL1fullload = true;
        self->ctx.kBL1fullload = true;
        return;
    }
    self->ctx.maxKAL1Iter =
        CeilDIV(self->ctx.singleCoreCi * self->ctx.convTiling->kernelHxkernelW, self->ctx.convTiling->kAL1) - 1;
    self->ctx.maxKBL1Iter =
        CeilDIV(self->ctx.singleCoreCi * self->ctx.convTiling->kernelHxkernelW, self->ctx.convTiling->kBL1) - 1;
    self->ctx.ddr2l0LoopK = CeilDIV(AlignB(self->ctx.singleCoreCi * self->ctx.convTiling->kernelHxkernelW, Intf::k0),
                                    self->ctx.convTiling->kL0);
    self->ctx.maxKL0Iter = self->ctx.ddr2l0LoopK - 1;
    if constexpr(Intf::formatA == ConvFormat::NC1HWC0) {
        self->ctx.kAL1Tail = (AlignB(self->ctx.singleCoreCi * self->ctx.convTiling->kernelHxkernelW, Intf::k0a)) % self->ctx.convTiling->kAL1;
        self->ctx.kAL1Tail = self->ctx.kAL1Tail == 0 ? self->ctx.convTiling->kAL1 : self->ctx.kAL1Tail;
    } else {
        self->ctx.kAL1Tail = (self->ctx.singleCoreCi * self->ctx.convTiling->kernelHxkernelW) % self->ctx.convTiling->kAL1;
        self->ctx.kAL1Tail = self->ctx.kAL1Tail == 0 ? self->ctx.convTiling->kAL1 : self->ctx.kAL1Tail;
    }
    if constexpr(Intf::formatB == ConvFormat::FRACTAL_Z) {
        self->ctx.kBL1Tail = (AlignB(self->ctx.singleCoreCi * self->ctx.convTiling->kernelHxkernelW, Intf::k0b)) % self->ctx.convTiling->kBL1;
        self->ctx.kBL1Tail = self->ctx.kBL1Tail == 0 ? self->ctx.convTiling->kBL1 : self->ctx.kBL1Tail;
    } else {
        self->ctx.kBL1Tail = (self->ctx.singleCoreCi * self->ctx.convTiling->kernelHxkernelW) % self->ctx.convTiling->kBL1;
        self->ctx.kBL1Tail = self->ctx.kBL1Tail == 0 ? self->ctx.convTiling->kBL1 : self->ctx.kBL1Tail;
    }
    self->ctx.kL0Tail = (AlignB(self->ctx.singleCoreCi *
        self->ctx.convTiling->kernelHxkernelW, Intf::k0)) % self->ctx.convTiling->kL0;
    self->ctx.kL0Tail = self->ctx.kL0Tail == 0 ? self->ctx.convTiling->kL0 : self->ctx.kL0Tail;
    self->ctx.multiKAL1 = CeilDIV(self->ctx.convTiling->kAL1, self->ctx.convTiling->kL0);
    self->ctx.multiKBL1 = CeilDIV(self->ctx.convTiling->kBL1, self->ctx.convTiling->kL0);
    if constexpr (Intf::kPreLoadFlag) {
        self->ctx.lastLoopKAL1StartPos = self->ctx.ddr2l0LoopK - self->ctx.multiKAL1;
        self->ctx.lastLoopKBL1StartPos = self->ctx.ddr2l0LoopK - self->ctx.multiKBL1;
    }
}

template <class Intf, uint32_t ImplType>
struct Init {
    static __aicore__ inline void call(Intf *self, const TConv2DTiling *__restrict convTiling, TPipe *tPipe)
    {
        self->ctx.convTiling = convTiling;
        self->ctx.pipe = GetTPipePtr();
        if ASCEND_IS_AIC {
            printf("ASCEND_IS_AIC\n");

            // [zj]ToDocheck
            self->ctx.enableBias = false;
            self->ctx.enableQuantPre = false;
            self->ctx.enableReluPre = false;
            self->ctx.enableEltAntiQuant = false;
            self->ctx.enableQuantPost = false;
            self->ctx.enableEltwise = false;
            self->ctx.mAL1UpdateFlag = true;
            self->ctx.iter = 0;
            self->ctx.kIter = 0;

            self->ctx.orgCi = self->ctx.convTiling->orgCi;
            self->ctx.orgHi = self->ctx.convTiling->orgHi;
            self->ctx.orgWi = self->ctx.convTiling->orgWi;
            self->ctx.orgCo = self->ctx.convTiling->orgCo;
            self->ctx.orgHo = self->ctx.convTiling->orgHo;
            self->ctx.orgWo = self->ctx.convTiling->orgWo;
            self->ctx.kernelH = self->ctx.convTiling->kernelH;
            self->ctx.kernelW = self->ctx.convTiling->kernelW;
            self->ctx.dilatedKernelH = 1 + (self->ctx.kernelH - 1) * self->ctx.convTiling->dilationH;
            self->ctx.dilatedKernelW = 1 + (self->ctx.kernelW - 1) * self->ctx.convTiling->dilationW;
            self->ctx.singleCoreCi = self->ctx.convTiling->singleCoreCi; // Cin
            uint64_t alignCinKhKwA =
                AlignB(self->ctx.convTiling->singleCoreCi * self->ctx.convTiling->kernelHxkernelW, Intf::k0a);
            uint64_t alignCinKhKwB =
                AlignB(self->ctx.convTiling->singleCoreCi * self->ctx.convTiling->kernelHxkernelW, Intf::k0b);
            self->ctx.kAL1fullload = alignCinKhKwA == self->ctx.convTiling->kAL1;
            self->ctx.kBL1fullload = alignCinKhKwB == self->ctx.convTiling->kBL1;
            self->ctx.singleCoreCo = self->ctx.convTiling->singleCoreCo; // Co

            // [zj]MHoWoL1L0
            if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
                InitValueMMode(self);
            } else {
                InitValueHWMode(self);
            }

            // [zj]l0al0bl0cl1al1bl1biasl0biasl1scaleLocalTensor
            InitBuffer<Intf>(self);
            self->ctx.iter = 0;
            // [zj]KL1L0
            InitKDirectionValue<Intf>(self);
            // [zj]CoL1L0
            InitCoDirectionValue<Intf>(self);
            InitHf32Mode<Intf>(self);
            // Tools
            InitSubApiParams<Intf>(self);

            if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                self->ctx.ciPerGroup = self->ctx.convTiling->orgCi / self->ctx.convTiling->groups;
                self->ctx.singleGroupOpt = self->ctx.convTiling->singleCoreGroupOpt;
                OptGroupCalcBL1LoadTimes<Intf>(self);
            }
        } else {
            if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                OptGroupVecInit<Intf>(self);
            }
        }
    }

    static __aicore__ inline void InitValueMMode(Intf *self)
    {
        self->ctx.mAL1 = self->ctx.convTiling->hoL1;
        self->ctx.mL0 = self->ctx.convTiling->hoL0;
        self->ctx.singleCoreM = self->ctx.convTiling->singleCoreHo;
        InitMDirectionValue<Intf>(self);
    }

    static __aicore__ inline void InitValueHWMode(Intf *self)
    {
        self->ctx.singleCoreHo = self->ctx.convTiling->singleCoreHo; // Ho
        self->ctx.singleCoreWo = self->ctx.convTiling->singleCoreWo; // Wo
        InitHoDirectionValue<Intf>(self);
        InitWoDirectionValue<Intf>(self);
    }
};

template <class Intf, uint32_t ImplType>
struct SetOrgFmapShape {
    static __aicore__ inline void call(Intf *self, uint64_t orgCi, uint64_t orgHi, uint64_t orgWi)
    {
        self->ctx.orgCi = orgCi;
        self->ctx.orgHi = orgHi;
        self->ctx.orgWi = orgWi;
    }
};

template <class Intf, uint32_t ImplType>
struct SetOrgWeightShape {
    static __aicore__ inline void call(Intf *self, uint64_t orgCo, uint64_t orgCi, uint64_t orgKh, uint64_t orgKw)
    {
        self->ctx.orgCo = orgCo;
        self->ctx.orgCi = orgCi;
        self->ctx.kernelH = orgKh;
        self->ctx.kernelW = orgKw;
    }
};

template <class Intf, uint32_t ImplType>
struct SetOrgOutputShape {
    static __aicore__ inline void call(Intf *self, uint64_t orgCo, uint64_t orgHo, uint64_t orgWo)
    {
        self->ctx.orgCo = orgCo;
        self->ctx.orgHo = orgHo;
        self->ctx.orgWo = orgWo;
    }
};

template <class Intf, uint32_t ImplType>
struct SetSingleFmapShape {
    static __aicore__ inline void call(Intf *self, uint64_t singleCi, uint64_t singleHi, uint64_t singleWi)
    {
        self->ctx.singleCoreCi = singleCi;
    }
};

template <class Intf, uint32_t ImplType>
struct SetSingleOutputShape {
    static __aicore__ inline void call(Intf *self, uint64_t singleCo, uint64_t singleHo, uint64_t singleWo)
    {
        if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            // [zj]WoWo
            self->ctx.singleCoreCo = singleCo;
            self->ctx.singleCoreHo = singleHo;
            if ASCEND_IS_AIC {
                // [zj]singleCoreCosingleCoreHo
                InitCoDirectionValue<Intf>(self);
                InitHoDirectionValue<Intf>(self);
                if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    OptGroupCalcBL1LoadTimes<Intf>(self);
                }
            } else {
                if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    OptGroupInitNValue<Intf>(self);
                    OptGroupInitIterValue<Intf>(self);
                }
            }
        }
    }

    static __aicore__ inline void call(Intf *self, uint64_t singleCo, uint64_t singleM)
    {
        if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            self->ctx.singleCoreCo = singleCo;
            self->ctx.singleCoreM = singleM;
            if ASCEND_IS_AIC {
                // [zj]singleCoreCosingleCoreM
                InitCoDirectionValue<Intf>(self);
                InitMDirectionValue<Intf>(self);
                if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    OptGroupCalcBL1LoadTimes<Intf>(self);
                }
            } else {
                if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                    OptGroupInitNValue<Intf>(self);
                    OptGroupInitIterValue<Intf>(self);
                }
            }
        }
    }
};

// [zj]tilingData
// 
template <class Intf, uint32_t ImplType>
struct SetFmapStartPosition {
    static __aicore__ inline void call(Intf *self, int64_t hiStartPos, int64_t wiStartPos)
    {
        if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::HW_MODE)) {
            if ASCEND_IS_AIC {
                self->ctx.hiStartPos = hiStartPos;
                self->ctx.wiStartPos = wiStartPos;
            }
        }
    }

    static __aicore__ inline void call(Intf *self, int64_t mStartPos)
    {
        if constexpr (Intf::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE)) {
            if ASCEND_IS_AIC {
                self->ctx.mStartPos = mStartPos;
            }
        }
    }
};

template <class Intf, uint32_t ImplType>
struct SetOptGroupParams {
    static __aicore__ inline void call(Intf *self, uint64_t singleGroups, uint64_t singleGroupOpt)
    {
        self->ctx.singleGroups = singleGroups;
        self->ctx.singleGroupOpt = singleGroupOpt;
        self->ctx.singleCoreCi = singleGroups * self->ctx.ciPerGroup;

        if ASCEND_IS_AIC {
            InitKDirectionValue<Intf>(self);
            if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                OptGroupCalcBL1LoadTimes<Intf>(self);
            }
        } else {
            if constexpr (Intf::groupType == static_cast<int8_t>(ConvGroupType::OPT_GROUP_CONV)) {
                OptGroupInitKValue<Intf>(self);
            }
        }
    }
};

}  // namespace Conv2dFunc
#endif // CONV2D_COMMON_FUNC_H