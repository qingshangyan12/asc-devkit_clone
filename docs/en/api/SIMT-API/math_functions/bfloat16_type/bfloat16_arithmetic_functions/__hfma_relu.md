# \_\_hfma\_relu

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T11:38:25.766Z pushedAt=2026-10-20T03:02:40.222Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16_arithmetic_functions/__hfma_relu_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_bf16.h"`.

For the input bfloat16 data x, y, and z, this API computes the result of x multiplied by y plus z, and rounds the result following the CAST\_RINT mode. Negative results are set to 0.

## Prototype

```cpp
bfloat16_t __hfma_relu(const bfloat16_t x, const bfloat16_t y, const bfloat16_t z)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |
| z | Input | Source operand. |

## Return Value

The value of x \* y + z. This API is not affected by the global saturation mode. The special values are as follows:

-   When x is ±inf and y is ±0, nan is returned.
-   When x is ±0 and y is ±inf, nan is returned.
-   When x\*y is +inf and z is -inf, nan is returned.
-   When x\*y is -inf and z is +inf, nan is returned.
-   When x\*y+z exceeds the maximum value of the corresponding type range, +inf is returned.
-   When x\*y+z is less than the minimum value of the corresponding type range, 0 is returned.
-   If any one of x, y, and z is nan, nan is returned.

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __global__ __launch_bounds__(1024) void KernelHfma_relu(bfloat16_t* dst, bfloat16_t* x, bfloat16_t* y, bfloat16_t* z)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hfma_relu(x[idx], y[idx], z[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelHfma_relu(__gm__ bfloat16_t* dst, __gm__ bfloat16_t* x, __gm__ bfloat16_t* y, __gm__ bfloat16_t* z)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hfma_relu(x[idx], y[idx], z[idx]);
    }
    ```
