# GetTiling

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-08T13:07:08.552Z pushedAt=2026-10-27T11:35:54.869Z nl=2 -->

## Description

Obtains the Tiling parameters.

## Prototype

```
int64_t GetTiling(optiling::TCubeTiling& tiling)
```

```
int64_t GetTiling(AscendC::tiling::TCubeTiling& tiling)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| tiling | Output | The Tiling structure stores the final tiling result. For details about the TCubeTiling structure, see [Table 1](TCubeTiling_struct.md#tcubetiling-struct).<br>**optiling::TCubeTiling**: The TCubeTiling structure with the optiling namespace, which is the Matmul TilingData defined on the host side.<br>**AscendC::tiling::TCubeTiling**: The TCubeTiling structure with the AscendC::tiling namespace, which is the Matmul TilingData defined on the kernel side and is used together with the development method that defines the TilingData structure using standard C++ syntax. |

## Return Value

If the return value is not -1, the Tiling calculation succeeds, and the user can use the value of this Tiling structure. If the return value is -1, the Tiling calculation fails, and this Tiling result cannot be used.

## Constraints

In the scenario where the Tiling calculation fails, to view the cause of the Tiling calculation failure, set the log level to WARNING and search for the keyword "MatmulApi Tiling" in the logs. In the scenario where the Tiling calculation succeeds, to view the parameter values of the Tiling structure, set the log level to INFO and search for the keyword "MatmulTiling" in the logs.

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MatmulApiTiling tiling(ascendcPlatform);
tiling.SetAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
tiling.SetBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
tiling.SetCType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
tiling.SetBiasType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
tiling.SetShape(1024, 1024, 1024);
tiling.SetOrgShape(1024, 1024, 1024);
tiling.SetBias(true);
tiling.SetBufferSpace(-1, -1, -1);
optiling::TCubeTiling tilingData;
int ret = tiling.GetTiling(tilingData); // Obtain the Tiling parameters.
```
