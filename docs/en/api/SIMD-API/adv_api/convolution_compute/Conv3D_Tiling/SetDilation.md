# SetDilation

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:19:53.465Z pushedAt=2026-10-29T02:14:34.191Z nl=2 -->

## Description

Sets the dilation information.

## Prototype

```
void SetDilation(int64_t dilationD, int64_t dilationH, int64_t dilationW)
```

## Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dilationD | Input | Dilation size in the D direction. |
| dilationH | Input | Dilation size in the H direction. |
| dilationW | Input | Dilation size in the W direction. |

## Return Value

None

## Constraints

This API can be optionally called before calling the GetTiling API. If this API is not called, dilationD=1, dilationH=1, and dilationW=1 by default.

## Examples

```
// Instantiate the Conv3D Api.
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
Conv3dTilingApi::Conv3dTiling conv3dApiTiling(ascendcPlatform);
conv3dApiTiling.SetDilation(dilationD, dilationH, dilationW);
```

