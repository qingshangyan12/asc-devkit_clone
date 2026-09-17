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
 * \file conv2d_tiling.cpp
 * \brief
 */

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include <cstdint>
#include "../../../lib/conv/conv2d/conv2d_tiling_base.h"
#include "../../../lib/conv/conv2d/conv2d_tiling.h"
#else
#include <cstdint>
#include "include/adv_api/conv/conv2d/conv2d_tiling_base.h"
#include "include/adv_api/conv/conv2d/conv2d_tiling.h"
#endif

namespace Conv2dTilingApi {
int64_t Conv2dTiling::GetTiling(optiling::TConv2DApiTiling& tiling)
{
    (void)(tiling);
    return 0;
}

int64_t Conv2dTiling::GetTiling(AscendC::tiling::TConv2DApiTiling& tiling)
{
    (void)(tiling);
    return 0;
}

int64_t Conv2dTiling::Compute()
{
    return 0;
}
} // namespace Conv2dTilingApi
