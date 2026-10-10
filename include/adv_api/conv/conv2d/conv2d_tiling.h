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
 * \file conv2d_tiling.h
 * \brief
 */

#ifndef ADV_API_CONV_CONV2D_CONV2D_TILING_H
#define ADV_API_CONV_CONV2D_CONV2D_TILING_H

#include <memory>

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "kernel_tiling/kernel_tiling.h"
#include "conv2d_tilingdata.h"
#include "conv2d_tiling_base.h"
#include "tiling/platform/platform_ascendc.h"
#else
#include "kernel_tiling/kernel_tiling.h"
#include "conv2d_tilingdata.h"
#include "conv2d_tiling_base.h"
#include "utils/tiling/platform/platform_ascendc.h"
#endif

namespace Conv2dTilingApi {
class Conv2dTiling : public Conv2dTilingBase {
public:
    /**
     * @brief Constructor with AscendC platform context
     * @param ascendcPlatform Reference to AscendC platform instance
     */
    explicit Conv2dTiling(const platform_ascendc::PlatformAscendC& ascendcPlatform)
        : Conv2dTilingBase(ascendcPlatform) {};

    /**
     * @brief Constructor with platform information
     * @param platform Platform information structure
     */
    explicit Conv2dTiling(const PlatformInfo& platform)
        : Conv2dTilingBase(platform) {};

    ~Conv2dTiling() override = default;

    /**
     * @brief Get tiling configuration for 2D convolution
     * @param tiling Reference to tiling structure to be filled
     * @return Status code indicating success or failure
     */
    int64_t GetTiling(optiling::TConv2DApiTiling& tiling) override;

    /**
     * @brief Get tiling configuration for 2D convolution (AscendC version)
     * @param tiling Reference to AscendC tiling structure to be filled
     * @return Status code indicating success or failure
     */
    int64_t GetTiling(AscendC::tiling::TConv2DApiTiling& tiling) override;

protected:
    int64_t Compute() override;

private:
    std::unique_ptr<ConvTilingAlgorithm> CreateAlgorithm();
    int64_t SetTilingResult();
    std::unique_ptr<ConvTilingAlgorithm> algo_;
};
} // namespace Conv2dTilingApi

#endif // ADV_API_CONV_CONV2D_CONV2D_TILING_H
