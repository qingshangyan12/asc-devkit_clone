# \_\_usad

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T07:07:07.894Z pushedAt=2026-10-22T07:13:50.366Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/integer_math_functions/__usad_res.md#id1 -->

## Description

The header file path is `"simt_api/device_functions.h"`.

For input data **x**, **y**, and **z**, computes the result of |x - y| + z, that is, the sum of the absolute value of the difference between the first two inputs and the third input.

## Prototype

```cpp
unsigned int __usad(unsigned int x, unsigned int y, unsigned int z)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand, of the uint32_t type. |
| y | Input | Source operand, of the uint32_t type. |
| z | Input | Source operand, of the uint32_t type. |

## Return Value

Returns the result of |x - y| + z.

## Constraints

None

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __global__ __launch_bounds__(1024) void KernelUsad(unsigned int* dst, unsigned int* x, unsigned int* y, unsigned int* z)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __usad(x[idx], y[idx], z[idx]);
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelUsad(__gm__ unsigned int* dst, __gm__ unsigned int* x, __gm__ unsigned int* y, __gm__ unsigned int* z)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __usad(x[idx], y[idx], z[idx]);
    }
    ```
