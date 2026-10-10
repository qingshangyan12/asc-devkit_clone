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
 * \file conv2d_tiling_base.h
 * \brief
 */

#ifndef ADV_API_CONV_CONV2D_CONV2D_TILING_BASE_H
#define ADV_API_CONV_CONV2D_CONV2D_TILING_BASE_H

#include <cstdint>

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "kernel_tiling/kernel_tiling.h"
#include "tiling/platform/platform_ascendc.h"
#include "conv2d_tilingdata.h"
#include "../../../impl/conv/common/conv_common.h"
#include "conv2d_tiling_util.h"
#include "conv2d_tiling_algorithm.h"
#include "../../matmul/matmul_tiling_base.h"
#else
#include "kernel_tiling/kernel_tiling.h"
#include "utils/tiling/platform/platform_ascendc.h"
#include "conv2d_tilingdata.h"
#include "conv2d_tiling_util.h"
#include "conv2d_tiling_algorithm.h"
#include "impl/adv_api/detail/conv/common/conv_common.h"
#include "adv_api/matmul/matmul_tiling_base.h"
#endif

namespace Conv2dTilingApi {
using namespace matmul_tiling;

class Conv2dTilingBase {
public:
    explicit Conv2dTilingBase(const platform_ascendc::PlatformAscendC& ascendcPlatform);

    explicit Conv2dTilingBase(const PlatformInfo& platform);

    virtual ~Conv2dTilingBase() = default;

    /**
     * @brief Interface to get tiling information
     * @param [in] tiling: reference to store the tiling information
     * @note the tiling of this function is in namespace optiling
     */
    virtual int64_t GetTiling(optiling::TConv2DApiTiling& tiling) = 0;

    /**
     * @brief Interface to get tiling information
     * @param [in] tiling: reference to store the tiling information
     * @note the tiling of this function is in global namespace
     */
    virtual int64_t GetTiling(AscendC::tiling::TConv2DApiTiling& tiling) = 0;

    /**
     * @brief Set the original input feature map shape dimensions
     * @param [in] orgCi: the original channel in shape of input
     * @param [in] orgHi: the original height shape of input
     * @param [in] orgWi: the original width shape of input
     */
    void SetOrgInputShape(int64_t orgCi, int64_t orgHi, int64_t orgWi);

    /**
     * @brief Set the original weight shape dimensions
     * @param [in] orgCo: the original cout shape of weight
     * @param [in] orgKh: the original kernelh shape of weight
     * @param [in] orgKw: the original kernelw shape of weight
     */
    void SetOrgWeightShape(int64_t orgCo, int64_t orgKh, int64_t orgKw);

    /**
     * @brief Set the single output shape dimensions
     * @param [in] singleCo: the channel out shape of single output
     * @param [in] singleM: the height out mul width out dimension shape of single output
     * @param [in] singleHo: the height out dimension shape of single output
     * @param [in] singleWo: the width out dimension shape of single output
     */
    void SetSingleOutputShape(int64_t singleCo, int64_t singleM);
    void SetSingleOutputShape(int64_t singleCo, int64_t singleHo, int64_t singleWo);

    /**
     * @brief Set the single weight shape dimensions
     * @param [in] singleCi: the channel in shape of single weight
     * @param [in] singleKh: the kernel height shape of single weight
     * @param [in] singleKw: the kernel width shape of single weight
     */
    void SetSingleWeightShape(int64_t singleCi, int64_t singleKh, int64_t singleKw);

    /**
     * @brief Set the input tensor type configuration
     * @param [in] pos: the tensor position identifier
     * @param [in] format: the data format of input tensor
     * @param [in] dtype: the data type of input tensor
     */
    void SetInputType(const ConvCommonApi::TPosition pos,
                      const ConvCommonApi::ConvFormat format,
                      const ConvCommonApi::ConvDtype dtype);

    /**
     * @brief Set the weight tensor type configuration
     * @param [in] pos: the tensor position identifier
     * @param [in] format: the data format of weight tensor
     * @param [in] dtype: the data type of weight tensor
     */
    void SetWeightType(const ConvCommonApi::TPosition pos,
                       const ConvCommonApi::ConvFormat format,
                       const ConvCommonApi::ConvDtype dtype);

    /**
     * @brief Set the bias tensor type configuration
     * @param [in] pos: the tensor position identifier
     * @param [in] format: the data format of bias tensor
     * @param [in] dtype: the data type of bias tensor
     */
    void SetBiasType(const ConvCommonApi::TPosition pos,
                     const ConvCommonApi::ConvFormat format,
                     const ConvCommonApi::ConvDtype dtype);

    /**
     * @brief Set the output tensor type configuration
     * @param [in] pos: the tensor position identifier
     * @param [in] format: the data format of output tensor
     * @param [in] dtype: the data type of output tensor
     */
    void SetOutputType(const ConvCommonApi::TPosition pos,
                       const ConvCommonApi::ConvFormat format,
                       const ConvCommonApi::ConvDtype dtype);

    /**
     * @brief Set the conv2d padding parameters for all dimensions
     * @param [in] padTop: the padding size at the top (height start)
     * @param [in] padBottom: the padding size at the bottom (height end)
     * @param [in] padLeft: the padding size at the left (width start)
     * @param [in] padRight: the padding size at the right (width end)
     */
    void SetPadding(int64_t padTop, int64_t padBottom,
                    int64_t padLeft, int64_t padRight);
    /**
     * @brief Set the conv2d dilation parameters
     * @param [in] dilationH: the dilation value along height dimension
     * @param [in] dilationW: the dilation value along width dimension
     */
    void SetDilation(int64_t dilationH, int64_t dilationW);

    /**
     * @brief Set the conv2d stride parameters
     * @param [in] strideH: the stride value along height dimension
     * @param [in] strideW: the stride value along width dimension
     */
    void SetStride(int64_t strideH, int64_t strideW);

    /**
     * @brief Set the conv2d group configuration
     * @param [in] groups: the number of groups for grouped convolution
     */
    void SetGroups(int64_t groups);

    /**
     * @brief Set the conv2d pre quant configuration
     * @param [in] mode: pre quant mode
     */
    void SetPreQuant(uint8_t mode);

    /**
     * @brief Set the conv2d pre relu configuration
     * @param [in] mode: pre relu mode
     */
    void SetPreRelu(uint8_t mode);

    /**
     * @brief Set the conv2d pre clip relu configuration
     * @param [in] mode: pre clip relu mode
     */
    void SetPreClipRelu(uint8_t mode);

    /**
     * @brief Set the conv2d eltwise configuration
     * @param [in] eltwiseOp: eltwise op
     * @param [in] eltwiseBrcEn: eltwise broadcast enable
     * @param [in] eltwiseDtype: eltwise data type
     */
    void SetEltwise(uint8_t eltwiseOp, bool eltwiseBrcEn,
                    const ConvCommonApi::ConvDtype eltwiseDtype);

    /**
     * @brief Set the conv2d anti quant configuration
     * @param [in] mode: anti quant mode
     */
    void SetAntiQuant(uint8_t mode);

    /**
     * @brief Set the conv2d post quant configuration
     * @param [in] mode: post quant mode
     */
    void SetPostQuant(uint8_t mode);

    /**
     * @brief Set the conv2d post relu configuration
     * @param [in] mode: post relu mode
     */
    void SetPostRelu(uint8_t mode);

    /**
     * @brief Set the conv2d post clip relu configuration
     * @param [in] mode: post clip relu mode
     */
    void SetPostClipRelu(uint8_t mode);

    /**
     * @brief Set the conv2d padding configuration
     * @param [in] mode: padding mode
     */
    void SetPadding(uint8_t mode);

protected:
    /**
     * @brief Copy the inner tiling result to the output tiling
     * @param [in] tiling: the output tiling
     */
    void SaveTilingResult(optiling::TConv2DApiTiling& tiling);

    virtual int64_t Compute() = 0;

protected:
    BufferInfo bufferInfo_;
    Conv2dArgs convArgs_;
    optiling::TConv2DApiTiling innerTilingResult_;
    ConvTilingAlgoType algoType_ = ConvTilingAlgoType::TILING_1_CUT_H_L0_CUT_N;
};
} // namespace Conv2dTilingApi

#endif // ADV_API_CONV_CONV2D_CONV2D_TILING_BASE_H
