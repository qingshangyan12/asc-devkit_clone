# asc\_shfl\_up

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T09:03:31.545Z pushedAt=2026-10-22T11:34:48.898Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/Warp_functions/Warp_shfl_functions/asc_shfl_up_res.md#id1 -->

## Description

The header file path is `"simt_api/device_warp_functions.h"` (for APIs other than the half, half2, bfloat16\_t, and bfloat16x2\_t types), `"simt_api/asc_fp16.h"` (for the half and half2 type APIs), and `"simt_api/asc_bf16.h"` (for the bfloat16\_t and bfloat16x2\_t type APIs).

Obtains the var value input for exchange by the thread that is delta positions ahead of the current thread within the Warp (current thread Lane ID - delta). If the target thread is inactive, an uninitialized value in the register is obtained. The width parameter is used to divide the threads within the Warp into groups. The width parameter sets the grouping width of the 32 threads participating in the exchange, with a default value of 32, meaning all threads form one group.

In the multi-group scenario (width less than 32), the exchange operation of each group is independent, and each thread obtains the var value of the thread that is delta positions ahead of the current thread within its own group. If the thread number delta positions ahead of the current thread, that is, Lane ID - delta, is less than the starting Lane ID of the group, the var value of the current thread is returned.

For example, when 32 active threads within a Warp call the asc\_shfl\_up\(LaneId, 2, 16\) API, the return value of each thread is the var value of the thread corresponding to the current thread Lane ID - 2, or the var value of the current thread.

**Figure 1**  asc\_shfl\_up result diagram

![](../../../figures/asc_shfl_up_result.png "asc_shfl_up result diagram")

## Prototype

```cpp
inline int32_t asc_shfl_up(int32_t var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline uint32_t asc_shfl_up(uint32_t var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline float asc_shfl_up(float var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline int64_t asc_shfl_up(int64_t var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline uint64_t asc_shfl_up(uint64_t var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline half asc_shfl_up(half var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline half2 asc_shfl_up(half2 var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline bfloat16_t asc_shfl_up(bfloat16_t var, uint32_t delta, int32_t width = warpSize)
```

```cpp
inline bfloat16x2_t asc_shfl_up(bfloat16x2_t var, uint32_t delta, int32_t width = warpSize)
```

## Parameters

**Table 1**  Parameters

| Parameter Name | Input/Output | Description |
| --- | --- | --- |
| var | Input | Input operand used by the thread for exchange. |
| delta | Input | Forward offset of the thread, relative to the current thread, whose var value is to be obtained. |
| width | Input | Grouping width of the threads participating in the exchange within the Warp, with a default value of 32. The value range of width is (0, 32], and width must be a multiple of 2. |

## Return Value

The var value of the specified thread within the Warp.

## Constraints

-   If the target thread is inactive, an uninitialized value in the register is obtained.
-   If delta \>=width, all threads return the var value of the current thread.
-   If width is not a multiple of 2 or exceeds 32, the return value is abnormal.

## Examples

For the complete sample, see [Sobel edge detection sample](../../../../../../examples/03_simt_api/02_features/01_api_features/03_warp_instruction/sobel_warp_shfl/README.md).

The following examples include two usages: Example 1 uses asc\_shfl\_up to obtain the input value of the thread that is offset forward by delta relative to the current thread within the Warp group; Example 2 uses asc\_shfl\_up to perform reduction summation within each Warp.

-   Example 1:

    SIMT programming scenario:

    ```cpp
    #include "simt_api/device_warp_functions.h"

    __global__ __launch_bounds__(1024) void KernelShflUp(int32_t* dst)
    {
         int idx = threadIdx.x + blockIdx.x * blockDim.x;
         int32_t laneId = idx % 32;
         // The return values of threads 0 to 15 are {0,1,0,1,2,3,4,5,6,7,8,9,10,11,12,13}.
         // The return values of threads 16 to 31 are {16,17,16,17,18,19,20,21,22,23,24,25,26,27,28,29}.

         int32_t result = asc_shfl_up(laneId, 2, 16);
         dst[idx] = result;
    }
    ```

    SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/device_warp_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelShflUp(__gm__ int32_t* dst)
    {
         // asc_vf_call parameter: dim3{1024, 1, 1}.
         int idx = threadIdx.x + blockIdx.x * blockDim.x;
         int32_t laneId = idx % 32;
         // The return values of threads 0 to 15 are {0,1,0,1,2,3,4,5,6,7,8,9,10,11,12,13}.
         // The return values of threads 16 to 31 are {16,17,16,17,18,19,20,21,22,23,24,25,26,27,28,29}.

         int32_t result = asc_shfl_up(laneId, 2, 16);
         dst[idx] = result;
    }
    ```

-   Example 2:

    SIMT programming scenario:

    ```cpp
    #include "simt_api/device_warp_functions.h"

    __global__ __launch_bounds__(1024) void KernelShflUpReduceSum(int32_t* dst)
    {
         int idx = threadIdx.x + blockIdx.x * blockDim.x;
         int32_t laneId = idx % 32;
         int32_t value = laneId;

         value += asc_shfl_up(value, 16, 32);
         value += asc_shfl_up(value, 8, 32);
         value += asc_shfl_up(value, 4, 32);
         value += asc_shfl_up(value, 2, 32);
         value += asc_shfl_up(value, 1, 32);

         dst[idx] = value; // The reduction sum result resides in the thread with laneId 31 in each Warp.
    }
    ```

    SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/device_warp_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void KernelShflUpReduceSum(__gm__ int32_t* dst)
    {
         // asc_vf_call parameter: dim3{1024, 1, 1}
         int idx = threadIdx.x + blockIdx.x * blockDim.x;
         int32_t laneId = idx % 32;
         int32_t value = laneId;

         value += asc_shfl_up(value, 16, 32);
         value += asc_shfl_up(value, 8, 32);
         value += asc_shfl_up(value, 4, 32);
         value += asc_shfl_up(value, 2, 32);
         value += asc_shfl_up(value, 1, 32);

         dst[idx] = value; // The reduction sum result resides in the thread with laneId 31 in each Warp.
    }
    ```
