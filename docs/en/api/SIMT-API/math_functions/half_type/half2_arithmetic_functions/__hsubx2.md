# \_\_hsubx2

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T01:14:17.293Z pushedAt=2026-10-22T03:23:49.567Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half2_arithmetic_functions/__hsubx2_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Calculates the difference of the corresponding components of two half2 values, with rounding following the CAST\_RINT mode.

## Prototype

```cpp
half2 __hsubx2(const half2 x, const half2 y)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

The difference of the corresponding components of the input data. This API is affected by the global saturation register. The special values are as follows:

<table>
  <tr>
    <th>x Component</th>
    <th>y Component</th>
    <th>Return Value in Non-saturation Mode</th>
    <th>Return Value in Saturation Mode</th>
  </tr>
  <tr>
    <td>inf</td>
    <td>Finite value</td>
    <td>±inf</td>
    <td>±ASCRT_MAX_NORMAL_FP16</td>
  </tr>
  <tr>
    <td>Finite value</td>
    <td>±inf</td>
    <td>∓inf</td>
    <td>∓ASCRT_MAX_NORMAL_FP16</td>
  </tr>
  <tr>
    <td>±inf</td>
    <td>±inf</td>
    <td>nan</td>
    <td>0</td>
  </tr>
  <tr>
    <td>±inf</td>
    <td>∓inf</td>
    <td>±inf</td>
    <td>±ASCRT_MAX_NORMAL_FP16</td>
  </tr>
  <tr>
    <td>±0</td>
    <td>∓0</td>
    <td>±0</td>
    <td>±0</td>
  </tr>
  <tr>
    <td colspan="2">x=y</td>
    <td>0</td>
    <td>0</td>
  </tr>
  <tr>
    <td>nan</td>
    <td>Any value</td>
    <td>nan</td>
    <td>0</td>
  </tr>
  <tr>
    <td>any value</td>
    <td>nan</td>
    <td>nan</td>
    <td>0</td>
  </tr>
  <tr>
    <td>ASCRT_MAX_NORMAL_FP16</td>
    <td>-ASCRT_MAX_NORMAL_FP16</td>
    <td>inf</td>
    <td>ASCRT_MAX_NORMAL_FP16</td>
  </tr>
  <tr>
    <td>-ASCRT_MAX_NORMAL_FP16</td>
    <td>ASCRT_MAX_NORMAL_FP16</td>
    <td>-inf</td>
    <td>-ASCRT_MAX_NORMAL_FP16</td>
  </tr>
</table>

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __global__ __launch_bounds__(1024) void simt_hsubx2(half* x, half* y, half* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        half2* input1 = (half2*)x;
        half2* input2 = (half2*)y;
        half2* out = (half2*)dst;
        out[idx] = __hsubx2(input1[idx], input2[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_hsubx2(__gm__ half2* x, __gm__ half2* y, __gm__ half2* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        dst[idx] = __hsubx2(x[idx], y[idx]);
    }

    __global__ __vector__ void compute_kernel(__gm__ half* x, __gm__ half* y, __gm__ half* dst, uint32_t input_total_length)
    {
        asc_vf_call<simt_hsubx2>(dim3(1024), (__gm__ half2*)x, (__gm__ half2*)y, (__gm__ half2*)dst, input_total_length);
    }
    ```
