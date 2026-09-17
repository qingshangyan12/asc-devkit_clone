/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#include "kernel_basic_intf.h"
#include "adv_api/conv/conv2d/conv2d_api.h"
#include "adv_api/conv/conv2d/conv2d_config.h"

using namespace AscendC;
using namespace ConvCommonApi;

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
constexpr ConvFormat inputSizeFormat = ConvFormat::ND;
constexpr ConvFormat biasFormat = ConvFormat::ND;
constexpr ConvFormat preQuantFormat = ConvFormat::ND;
constexpr ConvFormat preReluFormat = ConvFormat::ND;
constexpr ConvFormat preClipReluFormat = ConvFormat::ND;
constexpr ConvFormat antiQuantFormat = ConvFormat::ND;
constexpr ConvFormat postQuantFormat = ConvFormat::ND;
constexpr ConvFormat postReluFormat = ConvFormat::ND;
constexpr ConvFormat postClipReluFormat = ConvFormat::ND;
constexpr ConvFormat paddingFormat = ConvFormat::ND;

extern "C" __global__ __aicore__ void conv_transpose_inner(GM_ADDR inputSize,
                                                           GM_ADDR x,
                                                           GM_ADDR filter,
                                                           GM_ADDR bias,
                                                           GM_ADDR preQuant,
                                                           GM_ADDR preRelu,
                                                           GM_ADDR preClipRelu,
                                                           GM_ADDR eltwise,
                                                           GM_ADDR antiQuant,
                                                           GM_ADDR postQuant,
                                                           GM_ADDR postRelu,
                                                           GM_ADDR postClipRelu,
                                                           GM_ADDR padding,
                                                           GM_ADDR y,
                                                           GM_ADDR workspace,
                                                           GM_ADDR tiling)
{
    return;
}
