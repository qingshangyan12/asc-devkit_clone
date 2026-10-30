# SetPadding

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:22:55.529Z pushedAt=2026-10-29T02:14:34.209Z nl=2 -->

## Description

Sets the padding information.

## Prototype

```
void SetPadding(int64_t padHead, int64_t padTail, int64_t padUp, int64_t padDown, int64_t padLeft, int64_t padRight)
```

## Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| padHead | Input | Padding size before the D dimension. |
| padTail | Input | Padding size after the D dimension. |
| padUp | Input | Padding size above the H dimension. |
| padDown | Input | Padding size below the H dimension. |
| padLeft | Input | Padding size to the left of the W dimension. |
| padRight | Input | Padding size to the right of the W dimension. |

## Return Value

None

## Constraints

This API is optional and can be called before calling the GetTiling API. If this API is not called, the default values are padHead=0, padTail=0, padUp=0, padDown=0, padLeft=0, padRight=0.

## Examples

```
// Instantiate the Conv3D API.
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
Conv3dTilingApi::Conv3dTiling conv3dApiTiling(ascendcPlatform);
conv3dApiTiling.SetPadding(padHead, padTail, padUp, padDown, padLeft, padRight);
```

