# \_\_hmax\_nan

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T12:13:27.122Z pushedAt=2026-10-21T06:03:05.255Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: Not supported
- Atlas inference products Vector Core: Not supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Not supported
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half_compare_functions/__hmax_nan_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Obtains the maximum value of two input data. If either input is nan, the result is nan.

![](../../../../figures/zh-cn_formulaimage_0000002545918676.png)

## Prototype

```cpp
half __hmax_nan(const half x, const half y)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

The maximum value of the two input data. Special values are as follows:

| x value | y value | Return Value |
| --- | --- | --- |
| 0 | 0 | 0 |
| 0 | -0 | 0 |
| -0 | 0 | 0 |
| -0 | -0 | -0 |
| inf | normal value | inf |
| -inf | normal value | normal value |
| inf | inf | inf |
| inf | -inf | inf |
| -inf | -inf | -inf |
| ASCRT\_MAX\_NORMAL\_FP16 | normal value | ASCRT\_MAX\_NORMAL\_FP16 |
| -ASCRT\_MAX\_NORMAL\_FP16 | normal value | normal value |
| nan | any value | nan |
| any value | nan | nan |

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    __global__ __launch_bounds__(1024) void KernelHmax_nan(half* dst, half* x, half* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hmax_nan(x[idx], y[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelHmax_nan(__gm__ half* dst, __gm__ half* x, __gm__ half* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hmax_nan(x[idx], y[idx]);
    }
    ```
