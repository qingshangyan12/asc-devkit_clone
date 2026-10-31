# BatchMatmulGetTmpBufSize

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-08T12:11:13.690Z pushedAt=2026-10-27T07:44:32.763Z nl=2 -->

## Description

After BatchMatmul Tiling calls the [GetTiling](../Matmul_Tiling/GetTiling.md) API to obtain the Tiling parameters, it obtains the usage sizes of the L1 Buffer/Unified Buffer (UB)/L0C Buffer based on the Tiling structure information.

## Prototype

```
int32_t BatchMatmulGetTmpBufSize(optiling::TCubeTiling& tiling, matmul_tiling::SysTilingTempBufSize& bufSize)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| tiling | Input | BatchMatmul Tiling structure, that is, the TCubeTiling structure obtained from the BatchMatmulTiling object. |
| bufSize | Output | Obtains the usage sizes of the L1 Buffer/UB/L0C Buffer based on the TCubeTiling structure information.<br><br>The SysTilingTempBufSize structure is defined as shown in the following code. |

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
matmul_tiling::BatchMatmulTiling tiling(ascendcPlatform);
optiling::TCubeTiling tilingData;
...                                         // Initialize tilingData. For details, see the usage description of the MatmulTiling class.
    int ret = tiling.GetTiling(tilingData); // Obtain the Tiling parameters.
SysTilingTempBufSize bufSize;
BatchMatmulGetTmpBufSize(tilingData, bufSize);
```
