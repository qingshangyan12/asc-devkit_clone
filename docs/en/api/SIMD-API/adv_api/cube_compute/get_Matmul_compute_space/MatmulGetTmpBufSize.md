# MatmulGetTmpBufSize

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-08T12:12:46.014Z pushedAt=2026-10-27T08:16:36.820Z nl=2 -->

## Description

This API is used to obtain the usage size of the L1 Buffer/Unified Buffer (UB)/L0C Buffer based on the Tiling structure information, after the Tiling parameters are obtained by calling the [GetTiling](../Matmul_Tiling/GetTiling.md) API.

## Prototype

```
int32_t MatmulGetTmpBufSize(optiling::TCubeTiling& tiling, matmul_tiling::SysTilingTempBufSize& bufSize)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| tiling | Input | Structure of the Matmul single-core Tiling, that is, the **TCubeTiling** structure obtained from the **MatmulTiling** object. |
| bufSize | Output | Usage size of the L1 Buffer/UB/L0C Buffer in the Tiling.<br><br>The **SysTilingTempBufSize** structure is defined as shown in the following code. |

```
struct SysTilingTempBufSize {
    int32_t ubSize = 0;  // UB size.
    int32_t l1Size = 0;  // L1 Buffer size.
    int32_t l0cSize = 0; // L0C Buffer size.
};
```

## Return Value

**-1** indicates that the acquisition failed; **0** indicates that the acquisition succeeded.

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MatmulApiTiling tiling(ascendcPlatform);
optiling::TCubeTiling tilingData;
...                                         // Initialize tilingData. For details, see the usage description of the MatmulTiling class.
    int ret = tiling.GetTiling(tilingData); // Obtain the Tiling parameters.
SysTilingTempBufSize bufSize;
MatmulGetTmpBufSize(tilingData, bufSize);
```
