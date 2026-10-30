# SetInputShape

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:49:20.066Z pushedAt=2026-10-29T02:14:34.357Z nl=2 -->

## Description

Sets the shape of the feature matrix Input: Batch, Channel, Depth, Height, and Width. This API has no practical effect when constructing the Conv3DTranspose operator, so do not use it.

## Prototype

```
bool SetInputShape(int64_t n, int64_t c, int64_t d, int64_t h, int64_t w)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| n | Input | Batch value of Input. |
| c | Input | Channel value of Input. |
| d | Input | Depth value of Input. |
| h | Input | Height value of Input. |
| w | Input | Width value of Input. |

## Return Value

Returns true on success; otherwise, returns false.

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3DBpInputTiling conv3DBpDxTiling(*ascendcPlatform);
conv3DBpDxTiling.SetInputShape(n, c, d, h, w);
```
