# \_\_hmin\_nan

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T11:51:01.628Z pushedAt=2026-10-20T03:02:40.268Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16_compare_functions/__hmin_nan_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_bf16.h"`.

Obtains the minimum value of the two input data. If either input is nan, the result is **nan**.

![](../../../../figures/zh-cn_formulaimage_0000002574049837.png)

## Prototype

```cpp
bfloat16_t __hmin_nan(const bfloat16_t x, const bfloat16_t y)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

The minimum value of the two input data. This API is not affected by the global saturation mode. The special values are as follows:

-   When **x** is **-0** and **y** is **+0**, returns **x**.
-   When **x** is **+0** and **y** is **-0**, returns **y**.
-   When either **x** or **y** is **nan**, returns **nan**.
-   When either **x** or **y** is **-inf**, the return value is **-inf**.
-   When **x** is **inf**, the return value is **y**.
-   When **y** is **inf**, the return value is **x**.

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __global__ __launch_bounds__(1024) void KernelHmin_nan(bfloat16_t* dst, bfloat16_t* x, bfloat16_t* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hmin_nan(x[idx], y[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelHmin_nan(__gm__ bfloat16_t* dst, __gm__ bfloat16_t* x, __gm__ bfloat16_t* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hmin_nan(x[idx], y[idx]);
    }
    ```
