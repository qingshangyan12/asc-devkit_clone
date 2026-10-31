# SetCType

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-08T13:15:03.190Z pushedAt=2026-10-27T11:38:45.346Z nl=2 -->

## Description

Sets the position, data format, data type, and other information of the C cube. These settings must be consistent with those on the kernel side.

## Prototype

```
int32_t SetCType(TPosition pos, CubeFormat type, DataType dataType)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| pos | Input | Buffer position of the C cube. Can be set to **TPosition::GM** or **TPosition::VECIN**. |
| type | Input | Data format of the C cube. Can be set to **CubeFormat::ND**, **CubeFormat::NZ**, or **CubeFormat::ND_ALIGN**. |
| dataType | Input | Data type of the C cube. Can be set to **DataType::DT_FLOAT**, **DataType::DT_FLOAT16**, **DataType::DT_BFLOAT16**, **DataType::DT_INT8**, **DataType::DT_INT32**, **DataType::DT_FLOAT8_E4M3FN**, or **DataType::DT_HIFLOAT8**. |

## Return Value

-1 indicates that the setting failed; 0 indicates that the setting succeeded.

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MatmulApiTiling tiling(ascendcPlatform);

// Set the C cube with the buffer position GM, data format ND, data type float, and no transpose by default.
tiling.SetCType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
```
