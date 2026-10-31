# rnorm3df

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T09:54:28.716Z pushedAt=2026-10-20T10:34:15.532Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/float_math_functions/rnorm3df_res.md#id1 -->

## Description

The header file path is `"simt_api/math_functions.h"`.

Gets the reciprocal of the square root of a^2 + b^2 + c^2.

![](../../../figures/zh-cn_formulaimage_0000002516816367.png)

## Prototype

```cpp
inline float rnorm3df(float a, float b, float c)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| a | Input | Source operand. |
| b | Input | Source operand. |
| c | Input | Source operand. |

## Return Value

Returns the reciprocal of the square root of a^2 + b^2 + c^2.

-   If the square root of a^2 + b^2 + c^2 exceeds the maximum range of float, the return value is 0.

-   If the reciprocal of the square root of a^2 + b^2 + c^2 exceeds the maximum range of float, the return value is inf.

-   If **a**, **b**, and **c** are all 0, the return value is inf.

-   If any one or more of **a**, **b**, and **c** are ±inf, the return value is 0.

-   If any one or more of **a**, **b**, and **c** are NaN and none of them are ±inf, the return value is NaN.

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(256) void compute_rnorm3df(float *result, const float *x, const float *y, const float *z, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = rnorm3df(x[idx], y[idx], z[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(256) inline void compute_rnorm3df_vf(__gm__ float *result, __gm__ const float *x, __gm__ const float *y, __gm__ const float *z, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = rnorm3df(x[idx], y[idx], z[idx]);
    }

    __global__ __vector__ void run_rnorm3df(__gm__ float *result, __gm__ const float *x, __gm__ const float *y, __gm__ const float *z, uint32_t count)
    {
        asc_vf_call<compute_rnorm3df_vf>(dim3(256), result, x, y, z, count);
    }
    ```

The input and output examples are as follows:

```
x: 0.25, 0.75, 1.25, 1.75
y: 1.5, 2.5, 3.5, 4.5
z: -0.5, 0.5, 1.5, 2.5
result: 0.624695 0.3762884 0.2495131 0.1839202
```
