# SetGradOutputShape

<!-- md-trans-meta sourceCommit=8d4511303e7599d86c4c8f565e5b0a328eb60b1e translatedAt=2026-10-26T09:34:14.901Z pushedAt=2026-10-29T02:14:34.275Z nl=2 -->

## Description

Sets the shape of GradOutput: Batch, Channel, Depth, Height, and Width.

## Prototype

```
void SetGradOutputShape(int64_t n, int64_t c, int64_t d, int64_t h, int64_t w)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| n | Input | Batch value of the input GradOutput. |
| c | Input | Channel value of the input GradOutput. |
| d | Input | Depth value of the input GradOutput. |
| h | Input | Height value of the input GradOutput. |
| w | Input | Width value of the input GradOutput. |

## Return Value

None

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3dBpFilterTiling conv3dBpDwTiling(*ascendcPlatform);
conv3dBpDwTiling.SetGradOutputShape(n, c, d, h, w);
```
