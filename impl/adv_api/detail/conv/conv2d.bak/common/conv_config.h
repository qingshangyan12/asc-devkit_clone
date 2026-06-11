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
 * \file conv_config.h
 * \brief
 */

#ifndef CONV_CONFIG_H
#define CONV_CONFIG_H

#include "kernel_operator.h"
#include "kernel_tiling/kernel_tiling.h"
#include "kernel_utils.h"

namespace conv {
using namespace AscendC;

enum class ConvCfgTypeID {
    CONV_ID_Unknown = 0,
    CONV_ID_Normal,
    CONV_ID_Test,
    CONV_ID_END
};

enum class ConvFormat {
    ND = 0,
    NCHW,
    NHWC,
    NC1HWC0,
    FRACTAL_Z,
    FRACTAL_Z_C04,
    NDC1HWC0,
    FRACTAL_Z_3D,
    NCDHW
};

enum class ConvL1PingPong {
    ALL_CLOSE = 0,
    L1A_OPEN,
    L1B_OPEN,
    ALL_OPEN
};

enum class ConvFmpTilingReserved {
    DEFAULT_VAL = 0
};

enum class ConvFmpTiling {
    FULLLOAD_AL1 = 0,
    ONLY_N_FULLLOAD_AL1_AL0, // ONLY_M_FULLLOAD_AL1_AL0
    OTHER
};

enum class ConvOutputOrder {
    HW_MODE = 0,
    M_MODE
};

enum class ConvWeightTilingReserved {
    DEFAULT_VAL = 0
};

enum class ConvWeightTiling {
    FULLLOAD_BL1 = 0,
    ONLY_N_FULLLOAD_BL1_BL0,
    OTHER
};

enum class ConvHasBias {
    DEFAULT_VAL = 0
};

enum class ConvHasFixp {
    NON = 0,
    SCALE = 1,
    VECTOR = 2,
    UNDEF
};

enum class ConvIterOrder {
    ORDER_MTERFIRST = 0,
    ORDER_NTERFIRST,
    UNDEF,
};

enum class ConvL0PingPong {
    ALL_CLOSE = 0,
    L0A_OPEN,
    L0B_OPEN,
    ALL_OPEN
};

enum class ConvGroupType {
    NORMAL_CONV = 0,
    ORI_GROUP_CONV,
    OPT_GROUP_CONV
};

struct ConvParam {
    __aicore__ inline ConvParam(){};
    constexpr static int8_t l1PingPong = 0;
    constexpr static int8_t fmpTilingReserved = 0;
    constexpr static int8_t fmpTiling = 0;
    constexpr static int8_t outputOrder = static_cast<int8_t>(ConvOutputOrder::M_MODE);
    constexpr static int8_t weightTilingReserved = 0;
    constexpr static int8_t weightTiling = 0;
    constexpr static int8_t hasBias = 0;
    constexpr static int8_t iterOrder = 0;
    constexpr static int8_t l0PingPong = 0;
    constexpr static int8_t groupType = 0;
};

template <typename T>
struct GetDstType {
    using Type = T;
};

template <>
struct GetDstType<float> {
    using Type = float;
};

template <>
struct GetDstType<half> {
    using Type = float;
};

template <>
struct GetDstType<bfloat16_t> {
    using Type = float;
};

template <>
struct GetDstType<int8_t> {
    using Type = int32_t;
};

// template <>
// struct GetDstType<hifloat8_t> {
//     using Type = float;
// };

// template <>
// struct GetDstType<float8_e4m3_t> {
//     using Type = float;
// };

template <TPosition POSITION, ConvFormat FORMAT, typename TYPE>
struct ConvType {
    constexpr static TPosition pos = POSITION;
    constexpr static ConvFormat format = FORMAT;
    using T = TYPE;
};

template <class FMAP_TYPE, class WEIGHT_TYPE, class OUTPUT_TYPE, class BIAS_TYPE, class CONV_CFG>
struct ConvDataType {
    using ConvParam = CONV_CFG;
    using FmapT = typename FMAP_TYPE::T;
    using WeightT = typename WEIGHT_TYPE::T;
    using OutputT  = typename OUTPUT_TYPE::T;
    using BiasT = typename BIAS_TYPE::T;

    // [zj]ToDo: 
    constexpr static ConvFormat fixpParamsFormat = ConvFormat::ND;
    constexpr static ConvFormat eltFormat = ConvFormat::NC1HWC0;
    #define DTYPE_QUANT_PRE uint64_t
    #define DTYPE_RELU_PRE uint32_t
    #define DTYPE_ELT_ANTI_QUANT uint32_t
    #define DTYPE_QUANT_POST uint64_t
    #define DTYPE_ELTWISE int16_t
    using QUANT_PRE_TYPE = ConvType<TPosition::GM, fixpParamsFormat, DTYPE_QUANT_PRE>;
    using RELU_PRE_TYPE = ConvType<TPosition::GM, fixpParamsFormat, DTYPE_RELU_PRE>;
    using ELT_ANTI_QUANT_TYPE = ConvType<TPosition::GM, fixpParamsFormat, DTYPE_ELT_ANTI_QUANT>;
    using QUANT_POST_TYPE = ConvType<TPosition::GM, fixpParamsFormat, DTYPE_QUANT_POST>;
    using ELT_TYPE = ConvType<TPosition::GM, eltFormat, DTYPE_ELTWISE>;

    using QuantPreT = typename QUANT_PRE_TYPE::T;
    using ReluPreT = typename RELU_PRE_TYPE::T;
    using EltAntiQuantT = typename ELT_ANTI_QUANT_TYPE::T;
    using QuantPostT = typename QUANT_POST_TYPE::T;
    using EltwiseT = typename ELT_TYPE::T;

#if defined(__DAV_L510__)
    using L0cT = int32_t;
#else
    using L0cT = typename GetDstType<FmapT>::Type;
#endif

    constexpr static uint32_t configID = (uint32_t)ConvCfgTypeID::CONV_ID_Normal;
    constexpr static uint32_t implType = (uint32_t)ConvCfgTypeID::CONV_ID_Normal;
    constexpr static uint32_t intfType = (uint32_t)ConvCfgTypeID::CONV_ID_Normal;

    constexpr static auto formatA = FMAP_TYPE::format;
    constexpr static auto formatB = WEIGHT_TYPE::format;
    constexpr static auto formatC = OUTPUT_TYPE::format;
    constexpr static auto formatBias = BIAS_TYPE::format;
    constexpr static auto formatQuantPre = QUANT_PRE_TYPE::format;
    constexpr static auto formatReluPre = RELU_PRE_TYPE::format;
    constexpr static auto formatEltAntiQuant = ELT_ANTI_QUANT_TYPE::format;
    constexpr static auto formatQuantPost = QUANT_POST_TYPE::format;
    constexpr static auto formatEltwise = ELT_TYPE::format;

    constexpr static auto posA = FMAP_TYPE::pos;
    constexpr static auto posB = WEIGHT_TYPE::pos;
    constexpr static auto posC = OUTPUT_TYPE::pos;
    constexpr static auto posBias = BIAS_TYPE::pos;
    constexpr static auto posQuantPre = QUANT_PRE_TYPE::pos;
    constexpr static auto posReluPre = RELU_PRE_TYPE::pos;
    constexpr static auto posEltAntiQuant = ELT_ANTI_QUANT_TYPE::pos;
    constexpr static auto posQuantPost = QUANT_POST_TYPE::pos;
    constexpr static auto posEltwise = ELT_TYPE::pos;

    using ContextData = struct _ {
        __aicore__ inline _() {}
    };
};

template <class ConvDataType>
struct ConvConfig : public ConvDataType {
public:
    __aicore__ inline ConvConfig() {}

    using ContextData = struct _ : public ConvDataType::ContextData {
        __aicore__ inline _() {}
    };
};

}  // namespace conv
#endif
