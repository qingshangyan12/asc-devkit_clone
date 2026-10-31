# SetStride

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:24:18.320Z pushedAt=2026-10-29T02:14:34.218Z nl=2 -->

## Description

Sets the stride information.

## Prototype

```
void SetStride(int64_t strideD, int64_t strideH, int64_t strideW)
```

## Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| strideD | Input | Stride size in the D dimension. |
| strideH | Input | Stride size in the H dimension. |
| strideW | Input | Stride size in the W dimension. |

## Return Value

None

## Constraints

This API is optional and can be called before calling the GetTiling API. If this API is not called, the default values are strideD=1, strideH=1, and strideW=1.

## Examples

```
// Instantiate the Conv3D API.
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
Conv3dTilingApi::Conv3dTiling conv3dApiTiling(ascendcPlatform);
conv3dApiTiling.SetStride(strideD, strideH, strideW);
```

