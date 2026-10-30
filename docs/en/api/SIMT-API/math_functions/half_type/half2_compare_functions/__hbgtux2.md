# \_\_hbgtux2

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T01:20:49.203Z pushedAt=2026-10-22T03:24:41.963Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half2_compare_functions/__hbgtux2_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Compares the two components of two half2 values, and returns true when both components satisfy the condition that the first number is greater than the second number. If either input component is nan, the comparison result for that component is true.

## Prototype

```cpp
bool __hbgtux2(half2 x, half2 y)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

Returns the result of comparing whether each input component satisfies that the first number is greater than the second number. Special values are as follows:

<table>
  <tr>
    <th>x value</th>
    <th>y value</th>
    <th>Return Value</th>
  </tr>
  <tr>
    <td>(nan, nan)</td>
    <td>(nan, nan)</td>
    <td>true</td>
  </tr>
  <tr>
    <td>(nan, normal value)</td>
    <td>(normal value, nan)</td>
    <td>true</td>
  </tr>
  <tr>
    <td>(inf, inf)</td>
    <td>(finite value, finite value)</td>
    <td>true</td>
  </tr>
  <tr>
    <td>(-inf, -inf)</td>
    <td>(finite positive number, finite positive number)</td>
    <td>false</td>
  </tr>
  <tr>
    <td>(finite positive number, finite positive number)</td>
    <td>(inf, inf)</td>
    <td>false</td>
  </tr>
  <tr>
    <td>(inf, inf)</td>
    <td>(-inf, -inf)</td>
    <td>true</td>
  </tr>
  <tr>
    <td>(±0, ±0)</td>
    <td>(±0, ±0)</td>
    <td>false</td>
  </tr>
  <tr>
    <td colspan="2">All components of x are less than y.</td>
    <td>false</td>
  </tr>
  <tr>
    <td colspan="2">All components of x are equal to y.</td>
    <td>false</td>
  </tr>
</table>

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Using short vectors improves data transfer efficiency.
    __global__ __launch_bounds__(1024) void simt_hbgtux2(half* x, half* y, bool* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        half2* input1 = (half2*)x;
        half2* input2 = (half2*)y;
        dst[idx] = __hbgtux2(input1[idx], input2[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    // Using short vectors improves data transfer efficiency.
    __simt_vf__ __launch_bounds__(1024) inline void simt_hbgtux2(__gm__ half2* x, __gm__ half2* y, __gm__ bool* dst, uint32_t input_total_length)
    {
        uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        // Each thread processes one half2 value, that is, two half values. Therefore, threads with idx >= input_total_length / 2 do not process data.
        if (idx >= input_total_length / 2) {
            return;
        }
        dst[idx] = __hbgtux2(x[idx], y[idx]);
    }

    __global__ __vector__ void compare_kernel(__gm__ half* x, __gm__ half* y, __gm__ bool* dst, uint32_t input_total_length)
    {
        asc_vf_call<simt_hbgtux2>(dim3(1024), (__gm__ half2*)x, (__gm__ half2*)y, dst, input_total_length);
    }
    ```
