# \_\_brev

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T06:50:20.555Z pushedAt=2026-10-22T07:09:56.156Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/integer_math_functions/__brev_res.md#id1 -->

## Description

The header file path is `"simt_api/device_functions.h"`.

Reverses the bit order of the input and returns the reversed value.

## Prototype

```cpp
unsigned int __brev(unsigned int x)
```

```cpp
unsigned long long __brev(unsigned long long x)
```

```cpp
unsigned long __brev(unsigned long x)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| x | Input | Source operand. |

## Return Value

Returns the bit-reversed value of the input data. When the input type is uint32_t, the n-th bit of the return value corresponds to the (31-n)-th bit of the input data; when the input type is uint64_t, the n-th bit of the return value corresponds to the (63-n)-th bit of the input data.

- When x is 0 and the type is uint32\_t, the return value is 0.
- When x is 0 and the type is uint64\_t, the return value is 0.
- When x is 1 and the type is uint32\_t, the return value is 2147483648.
- When x is 1 and the type is uint64\_t, the return value is 9223372036854775808.

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __global__ __launch_bounds__(1024) void kernel_brev(unsigned int* dst, unsigned int* x, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = __brev(x[idx]);
    }
    ```

    ```cpp
    #include "simt_api/device_functions.h"

    __global__ __launch_bounds__(1024) void kernel_brev(unsigned long long* dst, unsigned long long* x, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = __brev(x[idx]);
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel_brev(__gm__ unsigned int* dst, __gm__ unsigned int* x, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = __brev(x[idx]);
    }
    ```

    ```cpp
    #include "simt_api/device_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel_brev(__gm__ unsigned long long* dst, __gm__ unsigned long long* x, uint32_t total_length)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (idx >= total_length) {
            return;
        }
        dst[idx] = __brev(x[idx]);
    }
    ```
