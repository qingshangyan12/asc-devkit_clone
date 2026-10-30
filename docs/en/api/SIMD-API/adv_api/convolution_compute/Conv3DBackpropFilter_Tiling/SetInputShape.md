# SetInputShape

<!-- md-trans-meta sourceCommit=8d4511303e7599d86c4c8f565e5b0a328eb60b1e translatedAt=2026-10-26T09:35:02.319Z pushedAt=2026-10-29T02:14:34.281Z nl=2 -->

## Description

Sets the shape of the feature matrix Input: Batch, Channel, Depth, Height, and Width.

## Prototype

```
void SetInputShape(int64_t n, int64_t c, int64_t d, int64_t h, int64_t w)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| n | Input | Batch value of the input Input. |
| c | Input | Channel value of the input Input. |
| d | Input | Depth value of the input Input. |
| h | Input | Height value of the input Input. |
| w | Input | Width value of the input Input. |

## Return Value

None

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3dBpFilterTiling conv3dBpDwTiling(*ascendcPlatform);
conv3dBpDwTiling.SetInputShape(n, c, d, h, w);
```
