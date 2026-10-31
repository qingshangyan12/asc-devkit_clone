# asc\_dcci\_single

<!-- md-trans-meta sourceCommit=343a6e89d2704a866eb5b4d939aacaa8ebe1820b translatedAt=2026-10-27T10:03:19.776Z pushedAt=2026-10-27T11:36:23.282Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/memory_access_functions/asc_dcci_single_res.md#id1 -->

## Description

The header file path is `"simt_api/device_functions.h"`.

This interface flushes the cache line where the specified address resides to ensure cache consistency when data is read.

In SIMT programming, written data is immediately written to Global Memory, making it visible to other cores, so there is no inter-core consistency issue. When reading data from Global Memory, the data may have been modified by other cores. In this case, use the **dcci** API to directly access Global Memory to obtain the latest data.

## Prototype

```cpp
inline void asc_dcci_single(void *dst)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Input | Global Memory address. |

## Return Value

None

## Constraints

The input must be a valid Global Memory address. When a non-Global Memory address or an invalid address is passed in, the Cache refresh behavior is not guaranteed to be effective.

## Examples

-   SIMT programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __global__ __launch_bounds__(1024) void kernelDcci_single(uint32_t* data, uint32_t* flag, uint32_t* output) {
        // Thread 0 of thread block 0 performs the data write operation.
        if (blockIdx.x == 0 && threadIdx.x == 0) {
            data[0] = 10;      // Write data.
            asc_threadfence(); // Ensure data write completion.
            flag[0] = 1;       // Flag indicating that the data is ready.
        }
 
        // Thread 0 of thread block 1 performs the data read operation.
        if (blockIdx.x == 1 && threadIdx.x == 0) {
            while (*(volatile uint32_t *)flag != 1) {
                asc_nop(); // Send the next load instruction after 15 cycles.
            }
            asc_dcci_single((void*)(data)); // Refresh the cache line where the specified address resides.
            output[0] = data[0]; // The data is ready. Obtain the data.
        }
    }
    ```

-   SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/device_functions.h"

    __simt_vf__ __launch_bounds__(1024) inline void kernelDcci_single(__gm__ uint32_t* data, __gm__ uint32_t* flag, __gm__ uint32_t* output)
    {
        // Thread 0 of thread block 0 performs the data write operation.
        if (blockIdx.x == 0 && threadIdx.x == 0) {
            data[0] = 10;      // Write data.
            asc_threadfence(); // Ensure data write completion.
            flag[0] = 1;       // Flag indicating that the data is ready.
        }
 
        // Thread 0 of thread block 1 reads data.
        if (blockIdx.x == 1 && threadIdx.x == 0) {
            while (*(volatile __gm__ uint32_t *)flag != 1) {
                asc_nop(); // Send the next load instruction after 15 cycles.
            }
            asc_dcci_single((__gm__ void*)(data)); // Refresh the cache line at the specified address.
            output[0] = data[0]; // The data is ready. Obtain the data.
        }
    }
    ```
