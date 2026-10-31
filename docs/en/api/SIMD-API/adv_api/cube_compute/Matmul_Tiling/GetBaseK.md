# GetBaseK

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-08T13:04:58.446Z pushedAt=2026-10-27T11:34:56.278Z nl=2 -->

## Description

Obtains the baseK value calculated by Tiling. For details about the baseK parameter, see [Table 1](TCubeTiling_struct.md#tcubetiling-struct).

## Prototype

```
int32_t GetBaseK() const
```

## Parameters

None

## Return Value

Returns the baseK value calculated by Tiling.

## Constraints

Call this API using the created Tiling object, and only after the Tiling calculation ([GetTiling](GetTiling.md)) is complete.

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
int ret = tiling.GetTiling(tilingData);
int baseK = tiling.GetBaseK(); // Obtain the baseK value calculated by Tiling.
```

