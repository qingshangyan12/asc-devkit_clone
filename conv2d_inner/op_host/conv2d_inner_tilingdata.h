/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#ifndef ASCENDC_CONV2D_INNER_TILING_H
#define ASCENDC_CONV2D_INNER_TILING_H

#include "tiling/tiling_templates_registry.h"
#include "register/tilingdata_base.h"
// [zj]TODO: 黄蓝归一
#include "lib/conv/conv2d/conv2d_tilingdata.h"

namespace optiling {
BEGIN_TILING_DATA_DEF(Conv2DRunInfo)
TILING_DATA_FIELD_DEF(uint64_t, hi);
TILING_DATA_FIELD_DEF(uint64_t, wi);
TILING_DATA_FIELD_DEF(uint64_t, ho);
TILING_DATA_FIELD_DEF(uint64_t, wo);
TILING_DATA_FIELD_DEF(uint32_t, batch);
TILING_DATA_FIELD_DEF(uint32_t, ci);
TILING_DATA_FIELD_DEF(uint32_t, co);
TILING_DATA_FIELD_DEF(uint32_t, kernelH);
TILING_DATA_FIELD_DEF(uint32_t, kernelW);
TILING_DATA_FIELD_DEF(uint32_t, batchDim);
TILING_DATA_FIELD_DEF(uint32_t, coDim);
TILING_DATA_FIELD_DEF(uint32_t, hoDim);
TILING_DATA_FIELD_DEF(uint32_t, strideH);
TILING_DATA_FIELD_DEF(uint32_t, padTop);
// [zj]TODO: group conv
// TILING_DATA_FIELD_DEF(uint32_t, groupDim);
// TILING_DATA_FIELD_DEF(uint32_t, groups);
// TILING_DATA_FIELD_DEF(uint32_t, enlarge);
// TILING_DATA_FIELD_DEF(uint32_t, cinOpt);
// TILING_DATA_FIELD_DEF(uint32_t, coutOpt);
// TILING_DATA_FIELD_DEF(uint32_t, groupOpt);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(Conv2DRunInfoOp, Conv2DRunInfo)

BEGIN_TILING_DATA_DEF(Conv2DTilingData)
TILING_DATA_FIELD_DEF_STRUCT(TConv2DApiTiling, conv2dApiTiling);
TILING_DATA_FIELD_DEF_STRUCT(Conv2DRunInfo, conv2dRunInfo);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(Conv2dInner, Conv2DTilingData)
} // namespace optiling

#endif // ASCENDC_CONV2D_INNER_TILING_H