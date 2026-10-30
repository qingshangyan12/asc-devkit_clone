# \_\_hmax

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T11:49:51.454Z pushedAt=2026-10-20T03:02:40.266Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16_compare_functions/__hmax_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_bf16.h"`.

Obtains the maximum value of the two input data.

![](../../../../figures/zh-cn_formulaimage_0000002576421057.png)

## Prototype

```cpp
inline bfloat16_t __hmax(bfloat16_t x, bfloat16_t y)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

Returns the maximum of the two input values. This API is not affected by the global saturation mode. The special values are as follows:

| x Value | y Value | Return Value |
| --- | --- | --- |
| +0 | -0 | +0 |
| -0 | +0 | +0 |
| nan | Non-special number (not nan, inf, or -inf) | y value |
| Non-special number (not nan, inf, or -inf) | nan | x value |
| Both x and y are nan | nan |  |
| Either x or y is +inf | +inf |  |
| -inf | Non-special number (not nan, inf, or -inf) | y |
| Non-special number (not nan, inf, or -inf) | -inf | x |

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __global__ __launch_bounds__(1024) void kernel_max(bfloat16_t* dst, bfloat16_t* x, bfloat16_t* y, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = __hmax(x[idx], y[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel_max(__gm__ bfloat16_t* dst, __gm__ bfloat16_t* x, __gm__ bfloat16_t* y, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = __hmax(x[idx], y[idx]);
    }
    ```
