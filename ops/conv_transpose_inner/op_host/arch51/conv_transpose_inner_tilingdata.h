/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#ifndef ASCENDC_CONV_TRANSPOSE_INNER_TILINGDATA_H
#define ASCENDC_CONV_TRANSPOSE_INNER_TILINGDATA_H

#if defined(ASCENDC_CANN) && (ASCENDC_CANN == 0)
#include "tiling/tiling_templates_registry.h"
#include "register/tilingdata_base.h"
#include "lib/conv/conv2d/conv2d_tilingdata.h"
#else
#include "tiling/tiling_templates_registry.h"
#include "register/tilingdata_base.h"
#include "adv_api/conv/conv2d/conv2d_tilingdata.h"
#endif

namespace optiling {
BEGIN_TILING_DATA_DEF(ConvTransposeRunInfo)
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
TILING_DATA_FIELD_DEF(uint32_t, groupDim);
TILING_DATA_FIELD_DEF(uint32_t, groups);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(ConvTransposeRunInfoOp, ConvTransposeRunInfo)

BEGIN_TILING_DATA_DEF(ConvTransposeInnerTilingData)
TILING_DATA_FIELD_DEF_STRUCT(TConv2DApiTiling, conv2dApiTiling);
TILING_DATA_FIELD_DEF_STRUCT(ConvTransposeRunInfo, convTransposeRunInfo);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(ConvTransposeInner, ConvTransposeInnerTilingData)
} // namespace optiling

#endif // ASCENDC_CONV_TRANSPOSE_INNER_TILINGDATA_H
