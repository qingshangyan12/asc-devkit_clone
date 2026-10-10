/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file conv2d_tiling_util.h
 * \brief conv2d tiling common types and utils
 */

#ifndef ADV_API_CONV_CONV2D_CONV2D_TILING_UTIL_H
#define ADV_API_CONV_CONV2D_CONV2D_TILING_UTIL_H
#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include <cstdint>
#include <map>
#include "conv2d_tilingdata.h"
#include "tiling/platform/platform_ascendc.h"
#include "../../../impl/conv/common/conv_common.h"
#else
#include <cstdint>
#include <map>
#include "conv2d_tilingdata.h"
#include "utils/tiling/platform/platform_ascendc.h"
#include "impl/adv_api/detail/conv/common/conv_common.h"
#include "impl/adv_api/tiling/conv/conv3d_tiling_util.h"
#endif
namespace Conv2dTilingApi {

using ConvDtype = ConvCommonApi::ConvDtype;
using ConvFormat = ConvCommonApi::ConvFormat;
using TPosition = ConvCommonApi::TPosition;

enum QuantMode_t {
    NoQuant = 0,
#if (defined __DAV_N350__)
    VREQ8 = 2,
    REQ8 = 3,
    VDEQF16 = 4,
    DEQF16 = 5,
    VREQ16 = 6,
    REQ16 = 7,
#else
    F322F16 = 1,
    VQF322HIF8_PRE = 2,
    QF322HIF8_PRE = 3,
    VQF322HIF8_PRE_HYBRID = 4,
    QF322HIF8_PRE_HYBRID = 5,
#if __CCE_AICORE__ < 300  // Available for V100 V200 V210 V220
    AttachF16Mul = 6,
#else
    VDEQS32_INT = 6,
    DEQS32_INT = 7,
#endif // __CCE_AICORE__ < 300
    VREQ8 = 8,
    REQ8 = 9,
    VDEQF16 = 10,
    DEQF16 = 11,
#if (defined __DAV_310R6__) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || \
    (__NPU_ARCH__ == 9202) || (__NPU_ARCH__ == 5102) || !(defined __CCE_AICORE__)
    VQF322FP8_PRE = 12,
    QF322FP8_PRE = 13,
    VQF322F32_PRE = 14,
    QF322F32_PRE = 15,
#endif
#if (!(defined __DAV_310R6__) && (__NPU_ARCH__ != 3101) && (__NPU_ARCH__ != 9201) && \
    (__NPU_ARCH__ != 9202) && (__NPU_ARCH__ != 5102)) || !(defined __CCE_AICORE__)
    VSHIFTS322S16 = 12,
    SHIFTS322S16 = 13,
#endif // if (defined __DAV_310R6__) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202) || (__NPU_ARCH__ == 5102)
    F322BF16 = 16,
    VQF162B8_PRE = 17,
    QF162B8_PRE = 18,
    VQF162S4_PRE = 19,
    QF162S4_PRE = 20,
    VREQ4 = 21,
    REQ4 = 22,
    VQF322B8_PRE = 23,
    QF322B8_PRE = 24,
    VQF322S4_PRE = 25,
    QF322S4_PRE = 26,
    VDEQS16 = 27,
    DEQS16 = 28,
    VQF162S16_PRE = 29,
    QF162S16_PRE = 30,
    VQF322F16_PRE = 31,
    QF322F16_PRE = 32,
    VQF322BF16_PRE = 33,
    QF322BF16_PRE = 34,
    VQS322BF16_PRE = 35,
    QS322BF16_PRE = 36,
#endif // if (defined __DAV_N350__)
};

enum ReluMode_t {
    NoRelu = 0,
    NormalRelu,
    ScalarRelu,
    VectorRelu,
    LUT_GELU,
    LUT_SILU,
    LUT_SIGMOID,
    LUT_TANH,
};

enum class QuantMode_post : uint8_t {
    NoConv = 0,
    VQS162B8_POST = 1,
    QS162B8_POST = 2,
    VQF162B8_POST = 3,
    QF162B8_POST = 4,
    VQS162S4_POST = 5,
    QS162S4_POST = 6,
    VQF162S4_POST = 7,
    QF162S4_POST = 8,
    VQS162S16_POST = 9,
    QS162S16_POST = 10,
    VQF162S16_POST = 11,
    QF162S16_POST = 12,
    VSHIFT2S4_POST = 13,
    SHIFT2S4_POST = 14,
    VSHIFT2S8_POST = 15,
    SHIFT2S8_POST = 16,
    VSHIFT2S16_POST = 17,
    SHIFT2S16_POST = 18,
};

enum class AntiQuantMode_t : uint8_t {
    S8_ANTIQ_SCALAR = 0,
    S8_ANTIQ_VECTOR = 1,
    S4_ANTIQ_SCALAR = 2,
    S4_ANTIQ_VECTOR = 3,
    U8_ANTIQ_SCALAR = 4,
    U8_ANTIQ_VECTOR = 5,
    S16_ANTIQ_SCALAR = 6,
    S16_ANTIQ_VECTOR = 7,
};

enum class EltwiseType : uint32_t { NoEltwise = 0, ADD, SUB, MUL, MAX, INVALID };

enum class ClipReLUMode {
    NoClipReLU = 0,
    ClipReLUScalar,
};

#ifndef FLOOR
#define FLOOR(N, n) ((N) / (n))
#endif
#ifndef ALIGN_FLOOR
#define ALIGN_FLOOR(N, n) ((N) / (n) * (n))
#endif

constexpr uint8_t pBufferL1APingPongPos = 0;
constexpr uint8_t pBufferL1BPingPongPos = 1;
constexpr uint8_t pBufferL0APingPongPos = 2;
constexpr uint8_t pBufferL0BPingPongPos = 3;
constexpr uint8_t pBufferL0MPingPongPos = 4;
constexpr uint8_t pBufferL1DualPos = 5;
constexpr uint8_t pBufferBl1ByPassPos = 6;

constexpr int64_t TILING_SUCCESS = 0;
constexpr int64_t TILING_FAILED = 0xFFFFFFFF;
constexpr int32_t ALIGN_SIZE = 32;
constexpr int32_t PT_SIZE = 256;

enum class CalType : uint32_t { FP16 = 0, S16S8 = 1, S8S8 = 2, FP8S8 = 3, S8S4 = 4, FP8S4 = 5, INVALID_VALUE };

enum class IterateOrder : uint8_t {
    MFirst = 0,
    NFirst = 1,
};
enum class ConvTilingAlgoType : int8_t {
    TILING_1_CUT_H_L0_CUT_N = 0,
    TILING_1_CUT_H_L0_CUT_KN,
    TILING_2_CUT_CO_L0_CUT_KN,
    TILING_5_CUT_HW_CO_L0_CUT_KN,
    TILING_7_CUT_H_CI_CO_L0_CUT_KN,
    TILING_1_CUT_H_L0_CUT_N_DIRECT_L0,
    TILING_1_CUT_H_L0_CUT_KN_DIRECT_L0,
    TILING_1_CUT_H_L0_CUT_KW_N_DIRECT_L0,
    TILING_5_CUT_HW_CO_L0_CUT_KH_KW_KN_DIRECT_L0,
    TILING_7_CUT_H_CI_CO_L0_CUT_KN_DIRECT_L0,
    TILING_1_CUT_H_L0_CUT_KW_N,          // 当前kernel没支持
    TILING_5_CUT_HW_CO_L0_CUT_KH_KW_KN,  // 当前kernel没支持
    TILING_ALGO_MAX
};

const std::map<ConvDtype, uint32_t> dtypeSizeMap = {
    {ConvDtype::FLOAT32, 4},
    {ConvDtype::FLOAT16, 2},
    {ConvDtype::INT16, 2},
    {ConvDtype::INT8, 1},
    {ConvDtype::UINT8, 1},
    {ConvDtype::INT4, 1},
    {ConvDtype::INT32, 4},
    {ConvDtype::INT64, 8},
};

struct BufferInfo {
    platform_ascendc::SocVersion socVersion;
    uint64_t blockDim = 1;
    uint64_t ubSize = 0;
    uint64_t l1Size = 0;
    uint64_t l0ASize = 0;
    uint64_t l0BSize = 0;
    uint64_t l0CSize = 0;
    uint64_t fbSize = 0;
    uint64_t btSize = 0;
    uint64_t ptSize = 0;
};

struct FixpipeInformation {
    QuantMode_t quantPreMode = NoQuant;
    ReluMode_t reluMode = NoRelu;
    ClipReLUMode clipReLUMode = ClipReLUMode::NoClipReLU;
    QuantMode_post quantPostMode = QuantMode_post::NoConv;
    ReluMode_t reluPostMode = NoRelu;
    AntiQuantMode_t antiQuantMode = AntiQuantMode_t::S8_ANTIQ_VECTOR;
    EltwiseType eltwiseOP = EltwiseType::NoEltwise;
    bool eltwiseAntiqEn = false;
    bool eltwiseBrcEn = false;
};

struct Conv2dArgs {
    ConvDtype featureMapType = ConvDtype::FLOAT16;
    ConvDtype weightType = ConvDtype::FLOAT16;
    ConvDtype biasType = ConvDtype::INT32;
    ConvDtype outputType = ConvDtype::FLOAT16;
    ConvDtype eltwiseType = ConvDtype::FLOAT16;
    ConvDtype partialType = ConvDtype::INT32;
    ConvFormat featureMapFormat = ConvFormat::NCHW;
    ConvFormat weightFormat = ConvFormat::NCHW;
    ConvFormat biasFormat = ConvFormat::NCHW;
    ConvFormat outputFormat = ConvFormat::NCHW;
    CalType computeType = CalType::FP16;
    int64_t hIn = 0;
    int64_t wIn = 0;
    int64_t singleHi = 0;
    int64_t singleWi = 0;
    int64_t hOut = 0;
    int64_t wOut = 0;
    int64_t singleHo = 0;
    int64_t singleWo = 0;
    int64_t batch = 0;
    int64_t cIn = 0;
    int64_t cOut = 0;
    int64_t singleCi = 0;
    int64_t singleCo = 0;
    int64_t kH = 0;
    int64_t kW = 0;
    int64_t strideH = 1;
    int64_t strideW = 1;
    int64_t dilationH = 1;
    int64_t dilationW = 1;
    int64_t kHDilated = 0;
    int64_t kWDilated = 0;
    int32_t padL = 0;
    int32_t padR = 0;
    int32_t padT = 0;
    int32_t padB = 0;
    int32_t groups = 1;
    bool isC04 = false;
    bool hasBias = false;
    bool hasQuantPre = false;
    bool hasReluPre = false;
    bool hasReluPost = false;
    bool hasPadding = false;
    bool hasEltAntiquant = false;
    bool hasQuantPost = false;
    bool hasEltwise = false;
    bool isFixpipeUpSample = false;
    bool isPerchannelPad = false;
    FixpipeInformation fixpipeInformation;
};

struct Conv2DBasicBlockInfo {
    uint32_t batch = 0;
    uint32_t ho = 0;
    uint32_t co = 0;
    uint32_t aicoreNum = 0;  // 这里实际不需要核数
};

}  // namespace Conv2dTilingApi

#endif  // ADV_API_CONV_CONV2D_CONV2D_TILING_UTIL_H
