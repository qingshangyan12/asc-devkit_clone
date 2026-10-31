# asc_copy_l12fb

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T12:39:39.788Z pushedAt=2026-10-28T12:45:45.405Z nl=2 -->

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
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Copies data from the L1 Buffer to the Fixpipe Buffer for subsequent in-line Vector quantization and in-line ReLU in the Fixpipe pipeline. The copied data consists of in-line Vector quantization and in-line ReLU parameters, which are stored independently and have independent address spaces. The quantization parameters and ReLU parameters are distinguished by the upper 4 bits of the dst address (`dst[19:16]`), as follows:

**Figure 1** Fixpipe Buffer diagram

![](../../../../figures/fixpipe_buffer_02.png)

Two call forms are provided:

- **High-dimensional split copy**: Specify `n_burst`, `len_burst`, `src_gap_size`, and `dst_gap_size` to implement contiguous and non-contiguous copies by configuring the number of data blocks, the length of each data block, and the source/destination gaps.

- **Copy the first n data blocks**: Specify only `size` to copy as one contiguous data block, with both source and destination gaps set to 0.

This API takes effect only when executed on the AIC.

## Prototype

- High-dimensional split copy

    ```c
    __aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size, uint16_t dst_gap_size)
    ```

- Copy the first n data blocks

    ```c
    __aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand, stored in the Fixpipe Buffer. `dst[15:0]` is the offset within the address space. Each address space is independent, and the address offset must be calculated separately. The destination address has different alignment requirements depending on the function.<br>&nbsp;&nbsp;&bull; Quantization scenario: `dst[16]=0` indicates the in-line quantization parameter address space, and the destination address must be 128B aligned.<br>&nbsp;&nbsp;&bull; ReLU scenario: `dst[16]=1` indicates the in-line ReLU parameter address space, and the destination address must be 64B aligned. |
| src | Input | Source operand, stored in the L1 Buffer. The source address must be 32-byte aligned. |
| n_burst | Input | Number of contiguous data blocks to copy. Value range: [1, 4095]. |
| len_burst | Input | Length of each contiguous data block, in units of 64 bytes. Value range: [1, 65535]. For the quantization scenario, the value must be a multiple of 2. |
| src_gap_size | Input | Stride coefficient between two adjacent copies on the source side (L1 Buffer), from the end of the previous copy to the start of the next copy, in units of 32 bytes. Value range: [0, 65535]. |
| dst_gap_size | Input | Stride coefficient between two adjacent copies on the destination side (Fixpipe Buffer), from the end of the previous copy to the start of the next copy, in units of 32 bytes. Value range: [0, 65535]. For the quantization scenario, the value must be a multiple of 2. |
| size | Input | Total size of the data to copy, in bytes.<br>**Note: size must be 32-byte aligned. If it is not aligned, the copy amount is rounded down to 32-byte alignment.** |

## Return Value

None

## Pipeline Type

PIPE_FIX

## Constraints

### General Constraints

- This API returns directly when it is not called by the AIC.
- The src start address must be 32-byte aligned (L1 Buffer alignment requirement); otherwise, a transfer exception is triggered.
- The dst start address has different alignment requirements depending on the function. For the quantization scenario, the start address must be 128-byte aligned; for the ReLU scenario, the start address must be 64-byte aligned. Otherwise, a transfer exception is triggered.
- The total L1 Buffer capacity is 512 KB. The sum of the src offset and the transfer size must not exceed the boundary; otherwise, an address out-of-bounds exception is triggered.
- For the Fixpipe Buffer, the capacity limit is 4 KB in the quantization scenario and 2 KB in the ReLU scenario. When the sum of the dst offset and the transfer size exceeds the boundary, the data is clipped, causing the transfer result to be unexpected.
- If the destination address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- For the quantization scenario, the values of `len_burst` and `dst_gap_size` must be a multiple of 2; otherwise, a transfer exception is triggered.
- When `n_burst` or `size` is set to `0`, no transfer is performed, and this API is treated as a NOP (no operation).

## Examples

After saving the code as `examples.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [`__NPU_ARCH__`](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id8 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng examples.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <cstdint>
#include <cstdio>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

constexpr uint32_t M = 128, N = 128, K = 128;
constexpr uint32_t BYTES = N * sizeof(half);

__global__ __cube__ void CopyL12fbKernel(__gm__ half* input)
{
    asc_init();
    __cbuf__ half l1[N];
    __fbuf__ half fbuf[N];
    asc_copy_gm2l1(l1, input, 1, BYTES / 32, asc_channel_pad_mode::NONE, 0, 0);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12fb(fbuf, l1, BYTES);
    asc_sync_pipe(PIPE_MTE1);
}

int main()
{
    aclInit(nullptr); aclrtSetDevice(0);
    half* input = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    CopyL12fbKernel<<<1, 0>>>(input);
    const aclError ret = aclrtSynchronizeDevice();
    std::printf("M=%u N=%u K=%u asc_copy_l12fb=%s\n", M, N, K,
        ret == ACL_SUCCESS ? "PASS" : "FAIL");
    aclrtFree(input); aclrtResetDevice(0); aclFinalize();
    return ret == ACL_SUCCESS ? 0 : 1;
}
```
