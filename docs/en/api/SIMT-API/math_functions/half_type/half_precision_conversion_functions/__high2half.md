# \_\_high2half

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-21T07:57:57.945Z pushedAt=2026-10-22T00:36:40.454Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half_precision_conversion_functions/__high2half_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Extracts the high 16 bits of the input **half2** and returns them.

## Prototype

```cpp
inline half __high2half(const half2 x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the high 16 bits of the input half2. Special values are as follows:

| x Value | Return Value |
| --- | --- |
| ±0 | ±0 |
| nan | nan |
| ±inf | ±inf |

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __aicore__ void simt_high2half(half2* input, half* output, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx > input_total_length / 2) {
            return;
        }
        output[idx] = __high2half(input[idx]);
    }
    __global__ __launch_bounds__(1024) void cast_kernel(half* input, half* output, uint32_t input_total_length)
    {
        simt_high2half((half2*)input, output, input_total_length);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_high2half(__gm__ half2* input, __gm__ half* output, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx > input_total_length / 2) {
            return;
        }
        output[idx] = __high2half(input[idx]);
    }
    __global__ __vector__ void cast_kernel(__gm__ half* input, __gm__ half* output, uint32_t input_total_length)
    {
        asc_vf_call<simt_high2half>(dim3(1024), (__gm__ half2*)input, output, input_total_length);
    }
    ```
