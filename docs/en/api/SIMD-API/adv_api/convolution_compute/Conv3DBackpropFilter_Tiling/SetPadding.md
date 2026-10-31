# SetPadding

<!-- md-trans-meta sourceCommit=8d4511303e7599d86c4c8f565e5b0a328eb60b1e translatedAt=2026-10-26T09:35:43.823Z pushedAt=2026-10-29T02:14:34.288Z nl=2 -->

## Description

Sets the padding information.

## Prototype

```
void SetPadding(int64_t padFront, int64_t padBack, int64_t padUp, int64_t padDown, int64_t padLeft, int64_t padRight)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| padFront | Input | Padding size in the front direction of the Input Depth dimension during the convolution forward process. |
| padBack | Input | Padding size in the back direction of the Input Depth dimension during the convolution forward process. |
| padUp | Input | Padding size in the up direction of the Input Height dimension during the convolution forward process. |
| padDown | Input | Padding size in the down direction of the Input Height dimension during the convolution forward process. |
| padLeft | Input | Padding size in the left direction of the Input Width dimension during the convolution forward process. |
| padRight | Input | Padding size in the right direction of the Input Width dimension during the convolution forward process. |

## Return Value

None

## Constraints

None

## Examples

```
optiling::Conv3DBackpropFilterTilingData tilingData;
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3dBpFilterTiling conv3dBpDwTiling(*ascendcPlatform);
conv3dBpDwTiling.SetPadding(padFront, padBack, padUp, padDown, padLeft, padRight);
```
