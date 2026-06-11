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
 * \file conv2d_config.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_CONFIG_H__
#endif

#ifndef ADV_API_CONV_CONV2D_CONV2D_CONFIG_H
#define ADV_API_CONV_CONV2D_CONV2D_CONFIG_H

#include "include/adv_api/conv/common/conv_forward_config.h"

namespace Conv2dApi {
enum class ConvFmapTiling : std::uint8_t {
    FULLLOAD_AL1 = 0,
    ONLY_M_FULLLOAD_AL1_AL0,
    OTHER
};

enum class ConvWeightTiling : std::uint8_t {
    FULLLOAD_BL1 = 0,
    ONLY_N_FULLLOAD_BL1_BL0,
    OTHER
};

enum class ConvL1PingPong : std::uint8_t {
    ALL_CLOSE = 0,
    AL1_OPEN,
    BL1_OPEN,
    ALL_OPEN
};

enum class ConvL0PingPong : std::uint8_t {
    ALL_CLOSE = 0,
    AL0_OPEN,
    BL0_OPEN,
    ALL_OPEN
};

enum class ConvOutputOrder : std::uint8_t {
    HW_MODE = 0,
    M_MODE
};

enum class ConvIterOrder : std::uint8_t {
    MITER_FIRST = 0,
    NITER_FIRST
};

enum class ConvBL1ByPass : std::uint8_t {
    FALSE = 0,
    TRUE
};

enum class ConvGroupType : std::uint8_t {
    NORMAL_CONV = 0,
    ORI_GROUP_CONV,
    OPT_GROUP_CONV
};

struct Conv2dParam{
    __aicore__ inline Conv2dParam() {};
    constexpr static int8_t fmapTiling = -1;
    constexpr static int8_t weightTiling = -1;
    constexpr static int8_t l1PingPong = -1;
    constexpr static int8_t l0PingPong = -1;
    constexpr static int8_t outputOrder = -1;
    constexpr static int8_t iterOrder = -1;
    constexpr static int8_t bl1ByPass = -1;
    constexpr static int8_t groupType = -1;
};

CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, fmapTiling, int8_t, INT8_MAX)
CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, weightTiling, int8_t, INT8_MAX)
CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, l1PingPong, int8_t, INT8_MAX)
CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, l0PingPong, int8_t, INT8_MAX)
CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, outputOrder, int8_t, INT8_MAX)
CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, iterOrder, int8_t, INT8_MAX)
CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, bl1ByPass, int8_t, INT8_MAX)
CONV_DECLARE_DEFINE_MEMBER(Conv2dParam, groupType, int8_t, INT8_MAX)

template <class INPUT_TYPE, class WEIGHT_TYPE, class OUTPUT_TYPE, class OUTPUT1_TYPE, class BIAS_TYPE,
          class PRE_QUANT_TYPE, class PRE_RELU_TYPE, class PRE_CLIP_RELU_TYPE,
          class ELTWISE_TYPE, class ANTI_QUANT_TYPE,
          class POST_QUANT_TYPE, class POST_RELU_TYPE, class POST_CLIP_RELU_TYPE,
          class CONV_CFG>
struct Conv2dDataType {
    using ConvParam = CONV_CFG;
    using FmapT = typename INPUT_TYPE::T;
    using WeightT = typename WEIGHT_TYPE::T;
    using OutputT = typename OUTPUT_TYPE::T;
    using Output1T = typename OUTPUT1_TYPE::T;
    using BiasT = typename BIAS_TYPE::T;
    using PreQuantT = typename PRE_QUANT_TYPE::T;
    using PreReluT = typename PRE_RELU_TYPE::T;
    using PreClipReluT = typename PRE_CLIP_RELU_TYPE::T;
    using EltwiseT = typename ELTWISE_TYPE::T;
    using AntiQuantT = typename ANTI_QUANT_TYPE::T;
    using PostQuantT = typename POST_QUANT_TYPE::T;
    using PostReluT = typename POST_RELU_TYPE::T;
    using PostClipReluT = typename POST_CLIP_RELU_TYPE::T;
    using L0cT = typename GetDstType<SrcT>::Type;

    constexpr static uint32_t configID = (uint32_t)ConvCfgTypeID::CONV_ID_Normal;
    constexpr static uint32_t implType = (uint32_t)ConvCfgTypeID::CONV_ID_Normal;
    constexpr static uint32_t intfType = (uint32_t)ConvCfgTypeID::CONV_ID_Normal;

    constexpr static auto formatFmap = INPUT_TYPE::format;
    constexpr static auto formatWeight = WEIGHT_TYPE::format;
    constexpr static auto formatOutput = OUTPUT_TYPE::format;
    constexpr static auto formatOutput1 = OUTPUT1_TYPE::format;
    constexpr static auto formatBias = BIAS_TYPE::format;
    constexpr static auto formatPreQuant = PRE_QUANT_TYPE::format;
    constexpr static auto formatPreRelu = PRE_RELU_TYPE::format;
    constexpr static auto formatPreClipRelu = PRE_CLIP_RELU_TYPE::format;
    constexpr static auto formatEltwise = ELTWISE_TYPE::format;
    constexpr static auto formatAntiQuant = ANTI_QUANT_TYPE::format;
    constexpr static auto formatPostQuant = POST_QUANT_TYPE::format;
    constexpr static auto formatPostRelu = POST_RELU_TYPE::format;
    constexpr static auto formatPostClipRelu = POST_CLIP_RELU_TYPE::format;

    constexpr static auto posFmap = INPUT_TYPE::pos;
    constexpr static auto posWeight = WEIGHT_TYPE::pos;
    constexpr static auto posOutput = OUTPUT_TYPE::pos;
    constexpr static auto posOutput1 = OUTPUT1_TYPE::pos;
    constexpr static auto posBias = BIAS_TYPE::pos;
    constexpr static auto posPreQuant = PRE_QUANT_TYPE::pos;
    constexpr static auto posPreRelu = PRE_RELU_TYPE::pos;
    constexpr static auto posPreClipRelu = PRE_CLIP_RELU_TYPE::pos;
    constexpr static auto posEltwise = ELTWISE_TYPE::pos;
    constexpr static auto posAntiQuant = ANTI_QUANT_TYPE::pos;
    constexpr static auto posPostQuant = POST_QUANT_TYPE::pos;
    constexpr static auto posPostRelu = POST_RELU_TYPE::pos;
    constexpr static auto posPostClipRelu = POST_CLIP_RELU_TYPE::pos;

    using ContextData = struct _ {
        __aicore__ inline _()
        {}
    };
};

template <class ConvDataType>
struct Conv2dCfg : public ConvApi::ConvConfig<ConvDataType> {
public:
    __aicore__ inline Conv2dCfg() {}

    using ContextData = struct _ : public ConvApi::ConvConfig<ConvDataType>::ContextData {
        __aicore__ inline _() {}
    };
};

} // namespace Conv2dApi

#endif // ADV_API_CONV_CONV2D_CONV2D_CONFIG_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_CONFIG_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_CONFIG_H__
#endif