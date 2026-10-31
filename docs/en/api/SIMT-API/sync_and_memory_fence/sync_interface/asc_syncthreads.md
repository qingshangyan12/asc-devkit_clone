# asc\_syncthreads

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T11:40:58.295Z pushedAt=2026-10-23T01:11:30.189Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/sync_and_memory_fence/sync_interface/asc_syncthreads_res.md#id1 -->

## Description

The header file path is `"simt_api/device_sync_functions.h"`.

In the SIMT (Single Instruction Multiple Threads) programming paradigm, multiple threads within a thread block execute in parallel, but their execution progress may differ. When threads need to coordinate work, share data, or ensure that certain operations execute in order, a synchronization mechanism is required.

asc\_syncthreads API is used to **block all threads within a thread block** until all threads reach the synchronization point.

**Key features:**

- This API blocks thread execution until all threads in the block reach the synchronization point.
- It ensures that all memory operations before the synchronization point are visible to all threads in the block.
- It is commonly used for data sharing, staged computation, and parallel reduction within a thread block.

The following figure illustrates the problems that may arise when multiple threads within a thread block share data:

![](../../../figures/syncthreads_1.png)

The execution flow of the asc\_syncthreads API is shown in the following figure:

![](../../../figures/syncthreads_function.png)

Note that all threads within a thread block must call the asc\_syncthreads\(\) API before the program can continue. If any thread does not reach the synchronization point, the other threads are blocked and wait, resulting in a deadlock. For example, placing a call to this API inside a branch may prevent some threads from reaching the API call site, causing the program to hang.

## Prototype

```cpp
inline void asc_syncthreads()
```

## Parameters

None

## Return Value

None

## Constraints

- All threads within a thread block must reach the synchronization point; otherwise, a deadlock occurs.

- Avoid calling this API inside a branch unless you can ensure that all threads within a thread block enter that branch.
- Avoid calling this API when the loop counts are inconsistent.

## Examples

For the complete sample, see [MemoryFence Sample](../../../../../../examples/03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence/README.md).

- SIMT programming scenario:

    ```cpp
    #include "simt_api/device_sync_functions.h"

    __global__ __launch_bounds__(1024) void kernel_sync_threads(float* dst, int count)
    {
         int idx = threadIdx.x;
         if (idx > 0 && idx < count) {
             dst[idx] = 1;
         }

         // Wait for all threads in the block to reach the current code.
         asc_syncthreads();

         if (idx == 0) {
             dst[0] = 0;
             for(int i = 1023; i > 0; i--) {
                 if (i < count) {
                     dst[0] += dst[i];
                 }
             }
         }
    }
    ```

    ```text
    Output:
    [1023, 1, 1, 1 …]
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/device_sync_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernel_sync_threads(__gm__ float* dst, int count)
    {
         int idx = threadIdx.x;
         if (idx > 0 && idx < count) {
             dst[idx] = 1;
         }

         // Wait for all threads in the block to reach the current code.
         asc_syncthreads();

         if (idx == 0) {
             dst[0] = 0;
             for(int i = 1023; i > 0; i--) {
                 if (i < count) {
                     dst[0] += dst[i];
                 }
             }
         }
    }
    ```

    ```text
    Output:
    [1023, 1, 1, 1 …]
    ```
