/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#ifndef ASCENDC_CONV2D_GROUP_H
#define ASCENDC_CONV2D_GROUP_H

#include "conv2d.h"

template <
    class FMAP_TYPE, class WEIGHT_TYPE, class OUTPUT_TYPE, class BIAS_TYPE, class PRE_QUANT_TYPE, class PRE_RELU_TYPE,
    class PRE_CLIP_RELU_TYPE, class ELTWISE_TYPE, class ANTI_QUANT_TYPE, class POST_QUANT_TYPE, class POST_RELU_TYPE,
    class POST_CLIP_RELU_TYPE, class PADDING_TYPE, class CONV_CFG>
class Conv2dGroup
    : public Conv2dBase<
          FMAP_TYPE, WEIGHT_TYPE, OUTPUT_TYPE, BIAS_TYPE, PRE_QUANT_TYPE, PRE_RELU_TYPE, PRE_CLIP_RELU_TYPE,
          ELTWISE_TYPE, ANTI_QUANT_TYPE, POST_QUANT_TYPE, POST_RELU_TYPE, POST_CLIP_RELU_TYPE, PADDING_TYPE, CONV_CFG> {
public:
    __aicore__ inline Conv2dGroup() {}

    __aicore__ inline void RunConv2dKernel(
        GM_ADDR famp_, GM_ADDR weight_, GM_ADDR bias_, GM_ADDR preQuant_, GM_ADDR preRelu_, GM_ADDR preClipRelu_,
        GM_ADDR eltwise_, GM_ADDR antiQuant_, GM_ADDR postQuant_, GM_ADDR postRelu_, GM_ADDR postClipRelu_,
        GM_ADDR padding_, GM_ADDR output_, const Conv2DInnerTilingData& conv2dTilingData, TPipe* tPipe)
    {
        return;
    }
};
#endif // ASCENDC_CONV2D_GROUP_H
