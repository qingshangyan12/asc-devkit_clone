# \_\_hmaxx2\_nan

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T13:56:41.913Z pushedAt=2026-10-20T03:02:40.566Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/bfloat16_type/bfloat16x2_compare_functions/__hmaxx2_nan_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_bf16.h"`.

Obtains the maximum value of each component of two bfloat16x2_t values. If any component is nan, the corresponding component of the result is nan.

## Prototype

```cpp
bfloat16x2_t __hmaxx2_nan(const bfloat16x2_t x, const bfloat16x2_t y)
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

Returns the maximum value of each component of the input data. The special values of the compared components x and y satisfy the following rules:

<table>
  <thead>
    <tr>
      <th>x Component Value</th>
      <th>y Component Value</th>
      <th>Return Value</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td colspan="2">Either component contains nan</td>
      <td>nan</td>
    </tr>
    <tr>
      <td>inf</td>
      <td>Non-nan</td>
      <td>inf</td>
    </tr>
    <tr>
      <td>non-nan</td>
      <td>inf</td>
      <td>inf</td>
    </tr>
    <tr>
      <td>inf</td>
      <td>-inf</td>
      <td>inf</td>
    </tr>
    <tr>
      <td>-inf</td>
      <td>any value</td>
      <td>y</td>
    </tr>
    <tr>
      <td>any value</td>
      <td>-inf</td>
      <td>x</td>
    </tr>
    <tr>
      <td>0</td>
      <td>-0</td>
      <td>0</td>
    </tr>
    <tr>
      <td>-0</td>
      <td>0</td>
      <td>0</td>
    </tr>
  </tbody>
</table>

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    // Use short vectors to improve data transfer efficiency.
    __global__ __launch_bounds__(1024) void simt_hmaxx2_nan(bfloat16_t* x, bfloat16_t* y, bfloat16_t* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one bfloat16x2_t value, that is, two bfloat16_t values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        bfloat16x2_t* input1 = (bfloat16x2_t*)x;
        bfloat16x2_t* input2 = (bfloat16x2_t*)y;
        bfloat16x2_t* out = (bfloat16x2_t*)dst;
        out[idx] = __hmaxx2_nan(input1[idx], input2[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_bf16.h"

    // Use short vectors to improve data transfer efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_hmaxx2_nan(__gm__ bfloat16x2_t* x, __gm__ bfloat16x2_t* y, __gm__ bfloat16x2_t* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one bfloat16x2_t value, that is, two bfloat16_t values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        dst[idx] = __hmaxx2_nan(x[idx], y[idx]);
    }

    __global__ __vector__ void compare_kernel(__gm__ bfloat16_t* x, __gm__ bfloat16_t* y, __gm__ bfloat16_t* dst, uint32_t input_total_length)
    {
        asc_vf_call<simt_hmaxx2_nan>(dim3(1024), (__gm__ bfloat16x2_t*)x, (__gm__ bfloat16x2_t*)y, (__gm__ bfloat16x2_t*)dst, input_total_length);
    }
    ```
