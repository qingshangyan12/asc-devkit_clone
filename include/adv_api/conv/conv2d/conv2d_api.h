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
 * \file conv2d_api.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_API_H__
#endif

#ifndef ADV_API_CONV_CONV2D_CONV2D_API_H
#define ADV_API_CONV_CONV2D_CONV2D_API_H

#include "conv2d_config.h"
#include "conv2d_intf.h"
#include "../../../../impl/adv_api/detail/conv/conv2d/conv2d_api_impl.h"

namespace Conv2dApi {
using fmapType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::NC1HWC0, int16_t>;
using weightType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::FRACTAL_Z, int8_t>;
using outputType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::NC1HWC0, int16_t>;
using output1Type = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::NC1HWC0, int16_t>;
using biasType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, int32_t>;
using preQuantType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, uint64_t>;
using preReluType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, uint32_t>;
using preClipReluType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, int16_t>;
using eltwiseType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::NC1HWC0, int16_t>;
using antiQuantType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, uint32_t>;
using postQuantType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, uint64_t>;
using postReluType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, uint32_t>;
using postClipReluType = ConvApi::ConvType<AscendC::TPosition::GM, ConvCommonApi::ConvFormat::ND, int16_t>;

template <class Config,
          template<typename, class> class Impl = Conv2dApiImpl,
          template<class, template<typename, class> class>class Intf = Conv2dIntf>
struct Conv2dIntfExt : public Intf<Config, Impl> {
    __aicore__ inline Conv2dIntfExt() {}
};

#define REGISTER_CONV_API(name, Config, Impl, Intf)                                                                   \
    template <class FMAP_TYPE = fmapType,                                                                             \
              class WEIGHT_TYPE = weightType,                                                                         \
              class OUTPUT_TYPE = outputType,                                                                         \
              class OUTPUT1_TYPE = output1Type,                                                                       \
              class BIAS_TYPE = biasType,                                                                             \
              class PRE_QUANT_TYPE = preQuantType,                                                                    \
              class PRE_RELU_TYPE = preReluType,                                                                      \
              class PRE_CLIP_RELU_TYPE = preClipReluType,                                                             \
              class ELTWISE_TYPE = eltwiseType,                                                                       \
              class ANTI_QUANT_TYPE = antiQuantType,                                                                  \
              class POST_QUANT_TYPE = postQuantType,                                                                  \
              class POST_RELU_TYPE = postReluType,                                                                    \
              class POST_CLIP_RELU_TYPE = postClipReluType,                                                           \
              class CONV_CFG=Conv2dParam>                                                                             \
    using name = Conv2dIntfExt<                                                                                       \
        Config<ConvDataType<FMAP_TYPE,                                                                                \
                            WEIGHT_TYPE,                                                                              \
                            OUTPUT_TYPE,                                                                              \
                            OUTPUT1_TYPE,                                                                              \
                            BIAS_TYPE,                                                                                \
                            PRE_QUANT_TYPE,                                                                           \
                            PRE_RELU_TYPE,                                                                            \
                            PRE_CLIP_RELU_TYPE,                                                                       \
                            ELTWISE_TYPE,                                                                             \
                            ANTI_QUANT_TYPE,                                                                          \
                            POST_QUANT_TYPE,                                                                          \
                            POST_RELU_TYPE,                                                                           \
                            POST_CLIP_RELU_TYPE,                                                                      \
                            CONV_CFG>>,                                                                               \
               Impl,                                                                                                  \
               Intf>

REGISTER_CONV_API(Conv2d, Conv2dCfg, Conv2dApiImpl, Conv2dIntf);

} // namespace Conv2dApi

#endif // ADV_API_CONV_CONV2D_CONV2D_API_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_API_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV2D_API_H__
#endif
