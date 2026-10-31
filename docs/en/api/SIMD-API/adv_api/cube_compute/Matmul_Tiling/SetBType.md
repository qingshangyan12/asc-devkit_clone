# SetBType

<!-- md-trans-meta sourceCommit=9627f141272dcd14deda8a6b4d33bfdeea41c662 translatedAt=2026-10-08T13:13:19.661Z pushedAt=2026-10-27T11:38:12.698Z nl=2 -->

## Description

Sets the position, data format, data type, and transpose flag of cube B. These settings must be consistent with those on the kernel side.

## Prototype

```
int32_t SetBType(TPosition pos, CubeFormat type, DataType dataType, bool isTrans = false)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| pos | Input | Buffer position of cube B. Can be set to **TPosition::GM**, **TPosition::VECOUT**, or **TPosition::TSCM**. |
| type | Input | Data format of cube B. Can be set to **CubeFormat::ND** or **CubeFormat::NZ**. |
| dataType | Input | Data type of cube B. Can be set to **DataType::DT_FLOAT**, **DataType::DT_FLOAT16**, **DataType::DT_BFLOAT16**, **DataType::DT_INT8**, **DataType::DT_INT4**, **DataType::DT_FLOAT8_E4M3FN**, **DataType::DT_FLOAT8_E5M2**, or **DataType::DT_HIFLOAT8**. |
| isTrans<a name="p47369411111"></a> | Input | Whether cube B is transposed.<br><br>Value range:<br>**true**: Cube B is transposed; **false**: Cube B is not transposed. |

## Return Value

-1 indicates a setting failure; 0 indicates a setting success.

## Constraints

None

<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/adv_api/cube_compute/Matmul_Tiling/SetBType_res.md#id1 -->

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MatmulApiTiling tiling(ascendcPlatform);
// Set cube B with the buffer position GM, data format ND, data type half, and no transpose by default.
tiling.SetBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
```
