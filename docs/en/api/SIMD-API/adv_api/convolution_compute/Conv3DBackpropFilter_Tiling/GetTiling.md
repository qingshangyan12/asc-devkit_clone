# GetTiling

<!-- md-trans-meta sourceCommit=8d4511303e7599d86c4c8f565e5b0a328eb60b1e translatedAt=2026-10-26T09:33:37.092Z pushedAt=2026-10-29T02:14:34.269Z nl=2 -->

## Description

Obtains the tiling parameters.

## Prototype

```
int64_t GetTiling(optiling::Conv3DBackpropFilterTilingData& tiling)
```

```
int64_t GetTiling(AscendC::tiling::Conv3DBackpropFilterTilingData& tiling)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| tiling | Output | Tiling structure of TConv3DBpFilterTiling, used to store the final tiling result. For details about the TConv3DBpFilterTiling structure, see [TConv3DBpFilterTiling Structure](TConv3DBpFilterTiling_struct.md). |

## Return Value

If the return value is not -1, the tiling computation succeeds and you can use the values in this tiling structure. If the return value is -1, the tiling computation fails and the tiling result cannot be used.

## Constraints

None

## Examples

```
#include "tiling/conv_backprop/conv3d_bp_filter_tiling.h"

optiling::Conv3DBackpropFilterTilingData tilingData;
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3dBpFilterTiling conv3dBpDwTiling(*ascendcPlatform);
conv3dBpDwTiling.SetWeightType(
    ConvCommonApi::TPosition::GM, ConvCommonApi::ConvFormat::FRACTAL_Z_3D, ConvCommonApi::ConvDtype::FLOAT32);
conv3dBpDwTiling.SetInputType(
    ConvCommonApi::TPosition::GM, ConvCommonApi::ConvFormat::NDC1HWC0, ConvCommonApi::ConvDtype::FLOAT16);
conv3dBpDwTiling.SetGradOutputType(
    ConvCommonApi::TPosition::GM, ConvCommonApi::ConvFormat::NDC1HWC0, ConvCommonApi::ConvDtype::FLOAT16);
conv3dBpDwTiling.SetInputShape(n, c, d, h, w);
conv3dBpDwTiling.SetGradOutputShape(n, c, d, h, w);
conv3dBpDwTiling.SetWeightShape(cout, cin, d, h, w);
conv3dBpDwTiling.SetPadding(padFront, padBack, padUp, padDown, padLeft, padRight);
conv3dBpDwTiling.SetStride(strideD, strideH, strideW);
conv3dBpDwTiling.SetDilation(dilationD, dilationH, dilationW);
int ret = conv3dBpDwTiling.GetTiling(tilingData); // Obtain the tiling parameters.
// Obtain the tiling parameters using the Tiling structure in the AscendC::tiling namespace.
AscendC::tiling::Conv3DBackpropFilterTilingData tilingDataNotOp;
ret = conv3dBpDwTiling.GetTiling(tilingDataNotOp);
```
