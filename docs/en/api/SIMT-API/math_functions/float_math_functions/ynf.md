# ynf

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T10:20:42.038Z pushedAt=2026-10-20T10:51:09.721Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/float_math_functions/ynf_res.md#id1 -->

## Description

The header file path is `"simt_api/math_functions.h"`.

Gets the value of the *n*th-order Bessel function of the second kind yn of input data x.

![](../../../figures/zh-cn_formulaimage_0000002516816391.png)

## Prototype

```cpp
inline float ynf(int n, float x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| n | Input | Source operand. |
| x | Input | Source operand. |

## Return Value

Returns the value of the Bessel function of the second kind y*n* for the input data.

-   When n < 0, the return value is nan.
-   When x = 0, the return value is -inf.
-   When x < 0, the return value is nan.
-   When x = inf, the return value is 0.
-   When x = nan, the return value is nan.
-   In fast computation mode (with [--cce-use-fast-math](../../../../guide/programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md#common-compilation-options) set to `true`), if the divisor computed internally is Subnormal data, the return value is ±inf.

## Constraints

-  The maximum value of **n** is 128.
-  This API is affected by the compilation option `--cce-use-fast-math`.
    -   --cce-use-fast-math=true: Enables fast computation mode. The internal implementation of this API uses the division operator. Because the division operator does not support the Subnormal scenario in fast computation mode, this API does not support the scenario where the divisor computed internally is Subnormal data.
    -   --cce-use-fast-math=false: Supports high-precision computation in the Subnormal scenario.

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(256) void compute_ynf(float *result, const int *n, const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = ynf(n[idx], x[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(256) inline void compute_ynf_vf(__gm__ float *result, __gm__ const int *n, __gm__ const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = ynf(n[idx], x[idx]);
    }

    __global__ __vector__ void run_ynf(__gm__ float *result, __gm__ const int *n, __gm__ const float *x, uint32_t count)
    {
        asc_vf_call<compute_ynf_vf>(dim3(256), result, n, x, count);
    }
    ```

The input and output examples are as follows:

```
n: 1, 2, 3, 1
x: 1, 2, 3, 4
result: -0.7812128 -0.6174081 -0.5385417 0.3979257
```
