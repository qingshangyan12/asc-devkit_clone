# \_\_isnanx2

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T02:05:59.602Z pushedAt=2026-10-22T06:31:40.114Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half2_compare_functions/__isnanx2_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Determines whether the two components of a half2 value are nan.

## Prototype

```cpp
half2 __isnanx2(half2 x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Determines whether each component of the input data is nan. If a component is nan, the corresponding component of the result is 1; otherwise, it is 0. Special values are as follows:

| Input | Return Value |
| --- | --- |
| nan | 1 |
| ±0 | 0 |
| ±inf | 0 |
| ASCRT\_MAX\_NORMAL\_FP16 | 0 |

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __global__ __launch_bounds__(1024) void simt_isnanx2(half* x, half* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        half2* input = (half2*)x;
        half2* out = (half2*)dst;
        out[idx] = __isnanx2(input[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_isnanx2(__gm__ half2* x, __gm__ half2* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        dst[idx] = __isnanx2(x[idx]);
    }

    __global__ __vector__ void compute_kernel(__gm__ half* x, __gm__ half* dst, uint32_t input_total_length)
    {
        asc_vf_call<simt_isnanx2>(dim3(1024), (__gm__ half2*)x, (__gm__ half2*)dst, input_total_length);
    }
    ```
