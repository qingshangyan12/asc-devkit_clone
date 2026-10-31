# \_\_hfmax2\_relu

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T01:09:21.145Z pushedAt=2026-10-22T03:22:57.875Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half2_arithmetic_functions/__hfmax2_relu_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Calculates the multiply-add result of the corresponding components of the input half2 values (the first two inputs are multiplied and then added to the third input), with rounding following the CAST\_RINT mode. Negative results are set to 0.

## Prototype

```cpp
half2 __hfmax2_relu(const half2 x, const half2 y, const half2 z)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |
| z | Input | Source operand. |

## Return Value

Multiply-add result of the corresponding components of the input data. This API is affected by the global saturation register. The special values are as follows:

<table>
  <tr>
    <th>x Component</th>
    <th>y Component</th>
    <th>z Component</th>
    <th>Return Value in Non-saturation Mode</th>
    <th>Return Value in Saturation Mode</th>
  </tr>
  <tr>
    <td colspan="3">Any value is nan</td>
    <td>nan</td>
    <td>0</td>
  </tr>
  <tr>
    <td>inf</td>
    <td>±0</td>
    <td>Any value</td>
    <td>nan</td>
    <td>0</td>
  </tr>
  <tr>
    <td colspan="2">x*y=±inf</td>
    <td>inf with opposite signs</td>
    <td>nan</td>
    <td>0</td>
  </tr>
  <tr>
    <td colspan="3">x*y+z is inf</td>
    <td>inf</td>
    <td>ASCRT_MAX_NORMAL_FP16</td>
  </tr>
  <tr>
    <td colspan="3">x*y+z ≤ 0</td>
    <td>0</td>
    <td>0</td>
  </tr>
</table>

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __global__ __launch_bounds__(1024) void simt_hfmax2_relu(half* x, half* y, half* z, half* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 data item, that is, two half data items. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        half2* input1 = (half2*)x;
        half2* input2 = (half2*)y;
        half2* input3 = (half2*)z;
        half2* out = (half2*)dst;
        out[idx] = __hfmax2_relu(input1[idx], input2[idx], input3[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Use short vectors to improve data transfer efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_hfmax2_relu(__gm__ half2* x, __gm__ half2* y, __gm__ half2* z, __gm__ half2* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 data item, that is, two half data items. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        dst[idx] = __hfmax2_relu(x[idx], y[idx], z[idx]);
    }

    __global__ __vector__ void compute_kernel(__gm__ half* x, __gm__ half* y, __gm__ half* z, __gm__ half* dst, uint32_t input_total_length)
    {
        asc_vf_call<simt_hfmax2_relu>(dim3(1024), (__gm__ half2*)x, (__gm__ half2*)y, (__gm__ half2*)z, (__gm__ half2*)dst, input_total_length);
    }
    ```
