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