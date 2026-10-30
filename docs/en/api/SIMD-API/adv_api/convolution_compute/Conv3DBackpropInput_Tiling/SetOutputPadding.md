# SetOutputPadding

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:50:04.256Z pushedAt=2026-10-29T02:14:34.363Z nl=2 -->

## Description

When constructing a Conv3DTranspose operator, this API sets the output padding size, which is used to derive the output shape. When constructing a Conv3DBackpropInput operator, this API has no practical effect and must not be used.

## Prototype

```
bool SetOutputPadding(int64_t outputPadD, int64_t outputPadH, int64_t outputPadW)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| outputPadD | Input | Padding value of the output in the depth direction. |
| outputPadH | Input | Padding value of the output in the height direction. |
| outputPadW | Input | Padding value of the output in the width direction. |

## Return Value

Returns true on success; otherwise, returns false.

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3DBpInputTiling conv3DBpDxTiling(*ascendcPlatform);
conv3DBpDxTiling.SetOutputPadding(outputPadD, outputPadH, outputPadW);
```
