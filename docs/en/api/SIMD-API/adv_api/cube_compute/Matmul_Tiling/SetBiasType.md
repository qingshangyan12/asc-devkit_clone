# SetBiasType

<!-- md-trans-meta sourceCommit=6a51d0b167dd8f5555456a0ebb783ef0427e366e translatedAt=2026-10-08T13:12:30.270Z pushedAt=2026-10-27T11:37:52.421Z nl=2 -->

## Description

Sets the position, data format, and data type of the Bias cube. These settings must be consistent with those on the kernel side.

## Prototype

```
int32_t SetBiasType(TPosition pos, CubeFormat type, DataType dataType)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| pos | Input | Buffer position of the Bias cube. Can be set to **TPosition::GM**, **TPosition::VECOUT**<!-- npu="950" id1 -->, or **TPosition::TSCM**<!-- end id1 -->. |
| type | Input | Data format of the Bias cube. Can be set to **CubeFormat::ND**. |
| dataType | Input | Data type of the Bias cube. Can be set to **DataType::DT_FLOAT**, **DataType::DT_FLOAT16**, **DataType::DT_INT32**<!-- npu="950" id2 -->, or **DataType::DT_BFLOAT16**<!-- end id2 -->.<br><br>Among them, the data type of Bias can be set to int32_t only when the data types of both A and B are int8_t. |

## Return Value

-1 indicates a setting failure; 0 indicates a setting success.

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MatmulApiTiling tiling(ascendcPlatform);
tiling.SetBiasType(
    matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT); // Set the Bias cube.
```
