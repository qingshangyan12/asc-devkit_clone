# \_\_hgeu

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-20T12:03:29.313Z pushedAt=2026-10-21T05:58:08.801Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/half_type/half_compare_functions/__hgeu_res.md#id1 -->

## Description

The header file path is `"simt_api/asc_fp16.h"`.

Compares two half values, and returns true if the first number is greater than or equal to the second number. If either input is NaN, returns true.

## Prototype

```cpp
bool __hgeu(half x, half y)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |
| y | Input | Source operand. |

## Return Value

Returns the result of comparing whether the first input value is greater than or equal to the second value. Special values are as follows:

<table>
  <tr>
    <th>x Value</th>
    <th>y Value</th>
    <th>Return Value</th>
  </tr>
  <tr>
    <td colspan="2">x&lt;y</td>
    <td>false</td>
  </tr>
  <tr>
    <td colspan="2">x&gt;y</td>
    <td>true</td>
  </tr>
  <tr>
    <td colspan="2">x=y</td>
    <td>true</td>
  </tr>
  <tr>
    <td colspan="2">Either value is nan.</td>
    <td>true</td>
  </tr>
  <tr>
    <td>±0</td>
    <td>±0</td>
    <td>true</td>
  </tr>
  <tr>
    <td>inf</td>
    <td>Finite value</td>
    <td>true</td>
  </tr>
  <tr>
    <td>-inf</td>
    <td>Finite value</td>
    <td>false</td>
  </tr>
</table>

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    __global__ __launch_bounds__(1024) void KernelHgeu(bool* dst, half* x, half* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hgeu(x[idx], y[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/asc_fp16.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelHgeu(__gm__ bool* dst, __gm__ half* x, __gm__ half* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __hgeu(x[idx], y[idx]);
    }
    ```
