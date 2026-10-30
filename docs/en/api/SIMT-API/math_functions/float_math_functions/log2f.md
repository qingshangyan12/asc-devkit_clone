# log2f

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T09:26:35.871Z pushedAt=2026-10-20T10:23:31.239Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/float_math_functions/log2f_res.md#id1 -->

## Description

The header file path is `"simt_api/math_functions.h"`.

Gets the logarithm of the input data to base 2.

![](../../../figures/zh-cn_formulaimage_0000002484776380.png)

## Prototype

```cpp
inline float log2f(float x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the base-2 logarithm of **x**.

- When x is inf, the return value is inf.
- When x is -inf, the return value is nan.
- When x is nan, the return value is nan.
- When x is 0, the return value is -inf.
- When **x** is any other negative number, the return value is nan.

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(256) void compute_log2f(float *result, const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = log2f(x[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(256) inline void compute_log2f_vf(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = log2f(x[idx]);
    }

    __global__ __vector__ void run_log2f(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        asc_vf_call<compute_log2f_vf>(dim3(256), result, x, count);
    }
    ```

The input and output examples are as follows:

```
x: 1, 2, 3, 4
result: 0 1 1.584962 2
```
