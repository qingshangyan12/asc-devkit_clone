# h2cos

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T14:06:36.560Z pushedAt=2026-10-20T03:02:40.583Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16x2_math_functions/h2cos_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_bf16.h"`.

Obtains the trigonometric cosine value of each input element.

![](../../../../figures/zh-cn_formulaimage_0000002545900882.png)

## Prototype

```cpp
inline bfloat16x2_t h2cos(bfloat16x2_t x)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the trigonometric cosine value of each element of the input data. This API is affected by the global saturation mode. The special values are as follows:

| Input | Non-saturation Mode Return Value | Saturation Mode Return Value |
|---|---|---|
| nan | nan | 0 |
| inf | nan | 0 |
| -inf | nan | 0 |

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __global__ __launch_bounds__(1024) void kernel_cos(bfloat16x2_t* dst, bfloat16x2_t* x, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = h2cos(x[idx]); // Obtain the trigonometric cosine value of the idx-th element at the source address x.
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel_cos(__gm__ bfloat16x2_t* dst, __gm__ bfloat16x2_t* x, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = h2cos(x[idx]); // Obtain the trigonometric cosine value of the idx-th element at the source address x.
    }
    ```
