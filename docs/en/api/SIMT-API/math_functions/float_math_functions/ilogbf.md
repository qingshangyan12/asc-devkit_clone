# ilogbf

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T09:11:00.871Z pushedAt=2026-10-20T10:06:39.523Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/float_math_functions/ilogbf_res.md#id1 -->

## Description

The header file path is `"simt_api/math_functions.h"`.

Calculate the logarithm of the absolute value of the input data to base 2, round the result down, and return an integer.

![](../../../figures/zh-cn_formulaimage_0000002484776384.png)

## Prototype

```cpp
inline int ilogbf(float x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

For finite non-zero values, returns the integer value obtained by computing the base-2 logarithm of the absolute value of x and rounding it down.

The return values in special value scenarios are shown in the following table.

| Input Value x | Return Value |
| --- | --- |
| +0 | ASCRT_MIN_VAL_S |
| -0 | ASCRT_MIN_VAL_S |
| inf | ASCRT_MAX_VAL_S |
| -inf | ASCRT_MAX_VAL_S |
| nan | ASCRT_MIN_VAL_S |

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(256) void compute_ilogbf(int *result, const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = ilogbf(x[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(256) inline void compute_ilogbf_vf(__gm__ int *result, __gm__ const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = ilogbf(x[idx]);
    }

    __global__ __vector__ void run_ilogbf(__gm__ int *result, __gm__ const float *x, uint32_t count)
    {
        asc_vf_call<compute_ilogbf_vf>(dim3(256), result, x, count);
    }
    ```

The input and output examples are as follows:

```
x: 0.25, 0.75, 1.25, 1.75
result: -2 -1 0 0
```
