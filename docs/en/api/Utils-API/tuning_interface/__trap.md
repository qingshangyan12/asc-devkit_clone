# \_\_trap

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T07:44:50.086Z pushedAt=2026-10-26T11:12:09.542Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/Utils-API/tuning_interface/__trap_res.md#id1 -->

## Description

Calling this API in SIMT code interrupts the execution of the operator. It is suitable for debugging abnormal scenarios on the kernel function (Kernel) side.

## Prototype

```cpp
inline void __trap()
```

## Parameters

None

## Return Value

None

## Constraints

None

## Required Header File

To use this API, include the "utils/debug/asc\_assert.h" header file.

```cpp
#include "utils/debug/asc_assert.h"
```

## Examples

-   SIMT programming scenario:

    ```cpp
    __global__ __launch_bounds__(1024) inline void SimtKernel(float* dst, float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (isnan(x[idx])) {
            __trap();
        }
        dst[idx] = x[idx];
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    __simt_vf__ __launch_bounds__(1024) inline void SimtKernel(__gm__ float* dst, __gm__ float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        if (isnan(x[idx])) {
            __trap();
        }
        dst[idx] = x[idx];
    }
    ```
