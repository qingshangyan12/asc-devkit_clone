# SetWeightType

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-26T09:36:36.123Z pushedAt=2026-10-29T02:14:34.297Z nl=2 -->

## Description

Sets the position, data format, and data type of the weight matrix Weight. These settings must be consistent with those on the kernel side.

## Prototype

```
void SetWeightType(ConvCommonApi::TPosition pos, ConvCommonApi::ConvFormat format, ConvCommonApi::ConvDtype dtype)
```

## Parameters

**Table 1**  Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| pos | Input | [Position](../../../general_description_and_constraints.md#table07372185712) of Weight in memory. Currently, only TPosition::GM is supported. |
| format | Input | Data format of Weight. Currently, only ConvFormat::FRACTAL_Z_3D is supported. |
| dtype | Input | Data type of Weight. Currently, only ConvDtype::FLOAT32 is supported. |

## Return Value

None

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
ConvBackpropApi::Conv3dBpFilterTiling conv3dBpDwTiling(*ascendcPlatform);
conv3dBpDwTiling.SetWeightType(
    ConvCommonApi::TPosition::GM, ConvCommonApi::ConvFormat::FRACTAL_Z_3D, ConvCommonApi::ConvDtype::FLOAT32);
```
