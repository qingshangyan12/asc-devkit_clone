
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#include "conv2d.h"
#include "conv2d_group.h"
#include "conv2d_inner_tilingkey.h"

using namespace AscendC;

#if defined(FORMAT_X) && FORMAT_X == FORMAT_NC1HWC0 && \
    defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
    defined(FORMAT_Y) && FORMAT_Y == FORMAT_NC1HWC0
    constexpr ConvFormat fmapFormat = ConvFormat::NC1HWC0;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NC1HWC0;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NC1HWC0 && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NCHW
    constexpr ConvFormat fmapFormat = ConvFormat::NC1HWC0;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NCHW;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NC1HWC0 && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NHWC
    constexpr ConvFormat fmapFormat = ConvFormat::NC1HWC0;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NHWC;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NCHW && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NC1HWC0
    constexpr ConvFormat fmapFormat = ConvFormat::NCHW;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NC1HWC0;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NCHW && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NCHW
    constexpr ConvFormat fmapFormat = ConvFormat::NCHW;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NCHW;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NCHW && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NHWC
    constexpr ConvFormat fmapFormat = ConvFormat::NCHW;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NHWC;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NHWC && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NC1HWC0
    constexpr ConvFormat fmapFormat = ConvFormat::NHWC;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NC1HWC0;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NHWC && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NCHW
    constexpr ConvFormat fmapFormat = ConvFormat::NHWC;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NCHW;
#elif defined(FORMAT_X) && FORMAT_X == FORMAT_NHWC && \
      defined(FORMAT_FILTER) && FORMAT_FILTER == FORMAT_FRACTAL_Z && \
      defined(FORMAT_Y) && FORMAT_Y == FORMAT_NHWC
    constexpr ConvFormat fmapFormat = ConvFormat::NHWC;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NHWC;
#else
    constexpr ConvFormat fmapFormat = ConvFormat::NC1HWC0;
    constexpr ConvFormat weightFormat = ConvFormat::FRACTAL_Z;
    constexpr ConvFormat outputFormat = ConvFormat::NC1HWC0;
#endif
#if defined(FORMAT_ELTWISE) && FORMAT_ELTWISE == FORMAT_NCHW
    constexpr ConvFormat eltwiseFormat = ConvFormat::NCHW;
#elif defined(FORMAT_ELTWISE) && FORMAT_ELTWISE == FORMAT_NHWC
    constexpr ConvFormat eltwiseFormat = ConvFormat::NHWC;
#else
    constexpr ConvFormat eltwiseFormat = ConvFormat::NC1HWC0;
#endif
constexpr ConvFormat biasFormat = ConvFormat::ND;
constexpr ConvFormat preQuantFormat = ConvFormat::ND;
constexpr ConvFormat preReluFormat = ConvFormat::ND;
constexpr ConvFormat preClipReluFormat = ConvFormat::ND;
constexpr ConvFormat antiQuantFormat = ConvFormat::ND;
constexpr ConvFormat postQuantFormat = ConvFormat::ND;
constexpr ConvFormat postReluFormat = ConvFormat::ND;
constexpr ConvFormat postClipReluFormat = ConvFormat::ND;
constexpr ConvFormat paddingFormat = ConvFormat::ND;
constexpr ConvFormat dynamicAippFormat = ConvFormat::ND;

template<int8_t FmapTiling=2, int8_t WeightTiling=2,
         int8_t L1PingPong=3, int8_t L0PingPong=3,
         int8_t OutputOrder=1, int8_t IterOrder=0,
         int Bl1ByPass=0, int8_t GroupType=0>
__aicore__ void inline conv2d_inner_(GM_ADDR x,
                                     GM_ADDR filter,
                                     GM_ADDR bias,
                                     GM_ADDR offsetW, // not support
                                     GM_ADDR preQuant,
                                     GM_ADDR preRelu,
                                     GM_ADDR preClipRelu,
                                     GM_ADDR eltwise,
                                     GM_ADDR antiQuant,
                                     GM_ADDR postQuant,
                                     GM_ADDR postRelu,
                                     GM_ADDR postClipRelu,
                                     GM_ADDR padding,
                                     GM_ADDR dynamicAipp, // not support now
                                     GM_ADDR y,
                                     GM_ADDR workspace,
                                     GM_ADDR tiling)
{
    SetSysWorkspace(workspace);
    GM_ADDR user = GetUserWorkspace(workspace);

    GET_TILING_DATA(tilingData, tiling);

    TPipe tPipe;

#if defined(DTYPE_X) && defined(DTYPE_FILTER) && defined(DTYPE_Y)
    using fmapType = ConvType<TPosition::GM,
                              fmapFormat,
                              DTYPE_X>;
    using weightType = ConvType<TPosition::GM,
                                weightFormat,
                                DTYPE_FILTER>;
    using outputType = ConvType<TPosition::GM,
                                outputFormat,
                                DTYPE_Y>;
    #if defined(DTYPE_BIAS)
        using biasType = ConvType<TPosition::GM,
                                  biasFormat,
                                  DTYPE_BIAS>;
    #else
        using biasType = ConvType<TPosition::GM,
                                  biasFormat,
                                  half>;
    #endif
    #if defined(DTYPE_PRE_QUANT)
        using preQuantType = ConvType<TPosition::GM,
                                      preQuantFormat,
                                      DTYPE_PRE_QUANT>;
    #else
        using preQuantType = ConvType<TPosition::GM,
                                      preQuantFormat,
                                      uint64_t>;
    #endif
    #if defined(DTYPE_PRE_RELU)
        using preReluType = ConvType<TPosition::GM,
                                     preReluFormat,
                                     DTYPE_PRE_RELU>;
    #else
        using preReluType = ConvType<TPosition::GM,
                                     preReluFormat,
                                     uint32_t>;
    #endif
    #if defined(DTYPE_PRE_CLIP_RELU)
        using preClipReluType = ConvType<TPosition::GM,
                                         preClipReluFormat,
                                         DTYPE_PRE_CLIP_RELU>;
    #else
        using preClipReluType = ConvType<TPosition::GM,
                                         preClipReluFormat,
                                         uint16_t>;
    #endif
    #if defined(DTYPE_ELTWISE)
        using eltwiseType = ConvType<TPosition::GM,
                                     eltwiseFormat,
                                     DTYPE_ELTWISE>;
    #else
        using eltwiseType = ConvType<TPosition::GM,
                                     eltwiseFormat,
                                     int16_t>;
    #endif
    #if defined(DTYPE_ANTI_QUANT)
        using antiQuantType = ConvType<TPosition::GM,
                                       antiQuantFormat,
                                       DTYPE_ANTI_QUANT>;
    #else
        using antiQuantType = ConvType<TPosition::GM,
                                       antiQuantFormat,
                                       uint32_t>;
    #endif
    #if defined(DTYPE_POST_QUANT)
        using postQuantType = ConvType<TPosition::GM,
                                       postQuantFormat,
                                       DTYPE_POST_QUANT>;
    #else
        using postQuantType = ConvType<TPosition::GM,
                                       postQuantFormat,
                                       uint64_t>;
    #endif
    #if defined(DTYPE_POST_RELU)
        using postReluType = ConvType<TPosition::GM,
                                      postReluFormat,
                                      DTYPE_POST_RELU>;
    #else
        using postReluType = ConvType<TPosition::GM,
                                      postReluFormat,
                                      uint32_t>;
    #endif
    #if defined(DTYPE_POST_CLIP_RELU)
        using postClipReluType = ConvType<TPosition::GM,
                                          postClipReluFormat,
                                          DTYPE_POST_CLIP_RELU>;
    #else
        using postClipReluType = ConvType<TPosition::GM,
                                          postClipReluFormat,
                                          uint16_t>;
    #endif
    #if defined(DTYPE_PADDING)
        using paddingType = ConvType<TPosition::GM,
                                     paddingFormat,
                                     DTYPE_PADDING>;
    #else
        using paddingType = ConvType<TPosition::GM,
                                     paddingFormat,
                                     uint16_t>;
    #endif
    #if defined(DTYPE_DYNAMIC_AIPP)
        using dynamicAippType = ConvType<TPosition::GM,
                                         dynamicAippFormat,
                                         DTYPE_DYNAMIC_AIPP>;
    #else
        using dynamicAippType = ConvType<TPosition::GM,
                                         dynamicAippFormat,
                                         uint64_t>;
    #endif

    if constexpr (GroupType == static_cast<int8_t>(Conv2dApi::ConvGroupType::NORMAL_CONV)) {
        Conv2dBase<fmapType, weightType, outputType, biasType,
                   preQuantType, preReluType, preClipReluType,
                   eltwiseType, antiQuantType,
                   postQuantType, postReluType, postClipReluType,
                   paddingType,
                   Param<FmapTiling, WeightTiling,
                         L1PingPong, L0PingPong,
                         OutputOrder, IterOrder,
                         Bl1ByPass, GroupType>>
            conv2dBase;
        conv2dBase.RunConv2dKernel(x, filter, bias,
                                   preQuant, preRelu, preClipRelu,
                                   eltwise, antiQuant,
                                   postQuant, postRelu, postClipRelu,
                                   padding, y, tilingData, &tPipe);
    } else {
        Conv2dGroup<fmapType, weightType, outputType, biasType,
                    preQuantType, preReluType, preClipReluType,
                    eltwiseType, antiQuantType,
                    postQuantType, postReluType, postClipReluType,
                    paddingType,
                    Param<FmapTiling, WeightTiling,
                          L1PingPong, L0PingPong,
                          OutputOrder, IterOrder,
                          Bl1ByPass, GroupType>>
            conv2dGroup;
        conv2dGroup.RunConv2dKernel(x, filter, bias,
                                    preQuant, preRelu, preClipRelu,
                                    eltwise, antiQuant,
                                    postQuant, postRelu, postClipRelu,
                                    padding, y, tilingData, &tPipe);
    }
#endif
}

#if !(defined(DEBUG) && (DEBUG == 1))
template<int8_t FmapTiling=2, int8_t WeightTiling=2,
         int8_t L1PingPong=2, int8_t L0PingPong=2,
         int8_t OutputOrder=1, int8_t IterOrder=0,
         int Bl1ByPass=0, int8_t GroupType=0>
__global__ __aicore__ void conv2d_inner(GM_ADDR x,
                                        GM_ADDR filter,
                                        GM_ADDR bias,
                                        GM_ADDR offsetW, // not support
                                        GM_ADDR preQuant,
                                        GM_ADDR preRelu,
                                        GM_ADDR preClipRelu,
                                        GM_ADDR eltwise,
                                        GM_ADDR antiQuant,
                                        GM_ADDR postQuant,
                                        GM_ADDR postRelu,
                                        GM_ADDR postClipRelu,
                                        GM_ADDR padding,
                                        GM_ADDR dynamicAipp, // not support now
                                        GM_ADDR y,
                                        GM_ADDR workspace,
                                        GM_ADDR tiling)
{
    conv2d_inner_<FmapTiling, WeightTiling, L1PingPong, L0PingPong, OutputOrder, IterOrder, Bl1ByPass, GroupType>(
        x, filter, bias, offsetW, preQuant, preRelu, preClipRelu, eltwise, antiQuant, postQuant, postRelu, postClipRelu,
        padding, dynamicAipp, y, workspace, tiling);
}
#else // just for debug， cann be deleted once tool support
extern "C" __global__ __aicore__ void conv2d_inner(GM_ADDR x,
                                                   GM_ADDR filter,
                                                   GM_ADDR bias,
                                                   GM_ADDR offsetW, // not support
                                                   GM_ADDR preQuant,
                                                   GM_ADDR preRelu,
                                                   GM_ADDR preClipRelu,
                                                   GM_ADDR eltwise,
                                                   GM_ADDR antiQuant,
                                                   GM_ADDR postQuant,
                                                   GM_ADDR postRelu,
                                                   GM_ADDR postClipRelu,
                                                   GM_ADDR padding,
                                                   GM_ADDR dynamicAipp, // not support now
                                                   GM_ADDR y,
                                                   GM_ADDR workspace,
                                                   GM_ADDR tiling)
{
#if defined(TILING_KEY_VAR) && (TILING_KEY_VAR == 00000000)
        conv2d_inner_<0, 0, 0, 0, 0, 0, 0, 0>(
            x, filter, bias, offsetW, preQuant, preRelu, preClipRelu, eltwise, antiQuant, postQuant, postRelu,
            postClipRelu, padding, dynamicAipp, y, workspace, tiling);
#endif
}
#endif