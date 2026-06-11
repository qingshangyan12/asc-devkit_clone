/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#ifndef CONV2D_GROUP_H
#define CONV2D_GROUP_H

#include "conv2d.h"

template <
    class FMAP_TYPE, class WEIGHT_TYPE, class OUTPUT_TYPE, class BIAS_TYPE, class preQuant_TYPE, class PRE_RELU_TYPE,
    class PRE_CLIP_RELU_TYPE, class ELTWISE_TYPE, class ANTI_QUANT_TYPE, class POST_QUANT_TYPE, class POST_RELU_TYPE,
    class POST_CLIP_RELU_TYPE, class CONV_CFG>
class Conv2dGroup
    : public Conv2dBase<
          FMAP_TYPE, WEIGHT_TYPE, OUTPUT_TYPE, BIAS_TYPE, preQuant_TYPE, PRE_RELU_TYPE, PRE_CLIP_RELU_TYPE,
          ELTWISE_TYPE, ANTI_QUANT_TYPE, POST_QUANT_TYPE, POST_RELU_TYPE, POST_CLIP_RELU_TYPE, CONV_CFG> {
public:
    __aicore__ inline Conv2dGroup() {}

    __aicore__ inline void RunConv2dKernel(
        GM_ADDR x, GM_ADDR filter, GM_ADDR bias, GM_ADDR preQuant, GM_ADDR preRelu, GM_ADDR preClipRelu,
        GM_ADDR eltwise, GM_ADDR antiQuant, GM_ADDR postQuant, GM_ADDR postRelu, GM_ADDR postClipRelu, GM_ADDR y,
        const Conv2DTilingData& conv2dTilingData, TPipe* tPipe)
    {
        return;
    }
}
#endif // CONV2D_GROUP_H