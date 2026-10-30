# EnableMultiCoreSplitK

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-08T13:04:17.033Z pushedAt=2026-10-27T11:34:47.472Z nl=2 -->

## Description

In a multi-core scenario, this API enables K-axis splitting. If this API is not called, K-axis splitting is disabled by default. Call this API before calling the **GetTiling** API.

## Prototype

```
void EnableMultiCoreSplitK(bool flag)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| flag | Input | Whether to enable K-axis splitting.<br>**true**: Enable K-axis splitting.<br>**false**: Disable K-axis splitting. |

## Return Value

None

## Constraints

-   When this API is used in an operator, the C cube result can only be output to Global Memory.
-   When this API is used in an operator, before the C cube tile result is first written to Global Memory in the kernel function code, Global Memory must be cleared first, and then AtomicAdd accumulation must be enabled when obtaining the C cube tile result. If Global Memory is not cleared in advance, precision issues may occur due to accumulation of the original invalid data in Global Memory.
-   When this API is used in an operator, Bias is not supported in cube multiplication.

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MultiCoreMatmulTiling tiling(ascendcPlatform);

tiling.EnableMultiCoreSplitK(true); // Enable K-axis splitting.
```
