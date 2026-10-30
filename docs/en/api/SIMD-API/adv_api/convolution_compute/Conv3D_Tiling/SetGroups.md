# SetGroups

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:20:24.776Z pushedAt=2026-10-29T02:14:34.194Z nl=2 -->

## Description

Sets the group size for grouped convolution. A group size of 1 indicates standard convolution. **The current high-level Conv3D API does not support grouped convolution.**

## Prototype

```
void SetGroups(int64_t groups)
```

## Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| groups | Input | Currently only the value 1 is supported, and grouped convolution is not supported. |

## Return Value

None

## Constraints

This API can be optionally called before calling the GetTiling API. If this API is not called, groups defaults to 1. Currently, only the value 1 is supported for groups, and the convolution capability of group\>1 is not supported.

## Examples

```
// Instantiate the Conv3D API.
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
Conv3dTilingApi::Conv3dTiling conv3dApiTiling(ascendcPlatform);
conv3dApiTiling.SetGroups(groups);
```
