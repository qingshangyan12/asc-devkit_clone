# y1f

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T10:19:06.532Z pushedAt=2026-10-20T10:50:24.257Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/float_math_functions/y1f_res.md#id1 -->

## Description

The header file path is `"simt_api/math_functions.h"`.

Gets the value of the first-order Bessel function of the second kind y1 for input data x.

![](../../../figures/zh-cn_formulaimage_0000002484776434.png)

## Prototype

```cpp
inline float y1f(float x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the value of the first-order Bessel function of the second kind y1 for input data x.

-   When x < 0, the return value is nan.
-   When x = 0, the return value is -inf.
-   When x = inf, the return value is 0.
-   When x = nan, the return value is nan.
-   In fast computation mode (with [--cce-use-fast-math](../../../../guide/programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md#common-compilation-options) set to `true`), if the divisor of an internal intermediate computed value is Subnormal data, the return value is -inf.

## Constraints

This API is affected by the compilation option `--cce-use-fast-math`.
-   --cce-use-fast-math=true: Enables fast computation mode. The internal implementation of this API uses the division operator. Because the division operator does not support the Subnormal scenario in fast computation mode, this API does not support the scenario where the divisor of an internal intermediate computed value is Subnormal data.
-   --cce-use-fast-math=false: Supports high-precision computation in the Subnormal scenario.

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(256) void compute_y1f(float *result, const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = y1f(x[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(256) inline void compute_y1f_vf(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = y1f(x[idx]);
    }

    __global__ __vector__ void run_y1f(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        asc_vf_call<compute_y1f_vf>(dim3(256), result, x, count);
    }
    ```


The input and output examples are as follows:

```
x: 1, 2, 3, 4
result: -0.7812128 -0.1070324 0.3246745 0.3979257
```
