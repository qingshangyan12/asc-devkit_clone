# SetWeightShape

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:51:02.882Z pushedAt=2026-10-29T02:14:34.372Z nl=2 -->

## Description

Sets the shape of the weight matrix Weight.

## Prototype

```
bool SetWeightShape(int64_t cout, int64_t cin, int64_t d, int64_t h, int64_t w)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| cout | Input | Sets the output channel size of the forward convolution, which is consistent with the Channel size of GradOutput. |
| cin | Input | Sets the input channel size of the forward convolution, which is consistent with the Channel size of GradInput. |
| d | Input | Sets the Depth value of weight. |
| h | Input | Sets the Height value of weight. |
| w | Input | Sets the Width value of weight. |

## Return Value

true indicates that the setting is successful, and false indicates that the setting fails.

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3DBpInputTiling con3dBpDxTiling(*ascendcPlatform);
con3dBpDxTiling.SetWeightShape(cout, cin, d, h, w);
```
