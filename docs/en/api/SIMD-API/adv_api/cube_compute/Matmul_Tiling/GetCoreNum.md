# GetCoreNum

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-08T13:06:21.728Z pushedAt=2026-10-27T11:35:27.811Z nl=2 -->

## Description

Obtains the **NumBlocks** parameter used for multi-core tiling.

## Prototype

-   **MultiCoreMatmulTiling** class

    ```
    int32_t GetCoreNum(int32_t& dim, int32_t& mDim, int32_t& nDim)
    ```

-   **BatchMatmulTiling** class

    ```
    int32_t GetCoreNum(int32_t& dim, int32_t& mDim, int32_t& nDim, int32_t& batchCoreM, int32_t& batchCoreN)
    ```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dim | Output | Gets the number of cores required for computation, dim = mDim * nDim. |
| mDim | Output | Gets the number of cores required in the M direction for computation. |
| nDim | Output | Gets the number of cores required in the N direction for computation.|
| batchCoreM | Output | Gets the number of cores required in the batch M direction for computation. Only supported by the **BatchMatmulTiling** class. |
| batchCoreN | Output | Gets the number of cores required in the batch N direction for computation. Only supported by the **BatchMatmulTiling** class. |

## Return Value

-1 indicates failure; 0 indicates success.

## Constraints

Call this API using the created Tiling object, and it must be called after the Tiling computation ([GetTiling](GetTiling.md)) is complete.

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MultiCoreMatmulTiling tiling(ascendcPlatform);
tiling.SetDim(1);
tiling.SetAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
tiling.SetBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
tiling.SetCType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
tiling.SetBiasType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
tiling.SetShape(1024, 1024, 1024);
tiling.SetSingleShape(1024, 1024, 1024);
tiling.SetOrgShape(1024, 1024, 1024);
tiling.SetBias(true);
tiling.SetBufferSpace(-1, -1, -1);
optiling::TCubeTiling tilingData;
int ret = tiling.GetTiling(tilingData);

// Obtain the NumBlocks used after multi-core tiling.
int32_t dim, mDim, nDim;
int ret1 = tiling.GetCoreNum(dim, mDim, nDim);
```
